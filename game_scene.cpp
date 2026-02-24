#include "game_scene.h"
#include "gui_image.h"
#include "default_entity.h"
#include "human_racer.h"
#include "cpu_racer.h"
#include "podium_scene.h"
#include "leaderboard_scene.h"
#include <algorithm>

GameScene::GameScene(GUIManager* gui, EntityManager* entities, Toolbox* toolbox) : Scene(gui, entities, toolbox){}

void GameScene::start() {
	track = toolbox->state.current_track;
	time = 0.0f;
	track->load();
	
	//Transition for when someone falls off
	this->respawn_transition = toolbox->transitions["respawn"];

	//beep beep beep BEEP sound
	intro_sound = toolbox->sound_manager->play_sound(toolbox->sounds[2],0.6f,0);

	//Hide the cursors in the game; they can become distracting
	gui_manager->show_cursors = false;
	toolbox->camera->fov = 70.0f;

	//Resources for the sky
	sky_shader = (SkyShader*)toolbox->shaders["sky"];
	human_players = toolbox->state.num_players;
	sky = toolbox->models["sky"];

	//Resources for the minimap
	square_texture = toolbox->gui["square"];
	car_map = new GUIImage(toolbox->gui["carmap"], glm::vec2(0), glm::vec2(1));
	car_map->num_sprites = 5;
	car_map->scale *= 0.3f;
	car_map->colour.a = 0.7f;

	//Countdown image, has 3 numbers
	countdown = new GUIImage(toolbox->gui["countdown"], glm::vec2(0), glm::vec2(1));
	countdown->num_sprites = 3;
	countdown->colour.a = 0;
	gui_manager->add_object(countdown);

	//Place image, has 8 sprites for places 1st to 8th
	place = new GUIImage(toolbox->gui["places"], glm::vec2(-5), glm::vec2(2));
	place->num_sprites = 8;

	//What lap you are on image. 1/3, 2/3, 3/3
	lap = new GUIImage(toolbox->gui["laps"], glm::vec2(-5), glm::vec2(2));
	lap->num_sprites = 3;

	//Add the track as an entity so it can be drawn, and scale it
	entity_manager->add_entity(new DefaultEntity(glm::vec3(0, -4, 0),
		track->track_model))->transform.scale *= track->scale;

	//Map is just a very squashed version of the collision model
	track_map = new DefaultEntity(glm::vec3(0, 0, 0), track->col_model);

	track_map->transform.scale *= track->scale;
	track_map->transform.scale.y = 0.1f;
	track_map->colour = glm::vec4(2, 2, 2, 0.6f);
	//Update it to set its world position and scale
	entity_manager->update_single_entity(track_map, 1.0f);

	//Add the dividers for the splitscreen
	if (human_players > 1) {
		GUIImage* divider = new GUIImage(toolbox->gui["dividerx"], glm::vec2(0, 0), glm::vec2(0, 0));
		divider->scale = glm::vec2(20, 0.05f);
		divider->colour = glm::vec4(0.847f, 0.431f, 0.8f, 1.0f);
		gui_manager->add_object(divider);
	}
	
	if (human_players > 2) {
		GUIImage* divider = new GUIImage(toolbox->gui["dividery"], glm::vec2(0, 0), glm::vec2(0, 0));
		divider->scale = glm::vec2(0.05f, 20);
		divider->colour = glm::vec4(0.847f, 0.431f, 0.8f, 1.0f);
		gui_manager->add_object(divider);
	}

	//Instantiate the human racers
	for (int i = 0; i < human_players; i++) {
		Racer* kart = (Racer*)entity_manager->add_entity(new HumanRacer(
			glm::vec3(
				track->start_pos.x * track->scale -i*6,
				track->start_pos.y * track->scale, 
				track->start_pos.z*track->scale
			), toolbox, toolbox->controllers[i], toolbox->camera, i));
		kart->transform.scale *= 5.0f;
		kart->add_child(new DefaultEntity(glm::vec3(0, 0, 0),
		 toolbox->characters[toolbox->state.player_characters[i]]->model)
		);
		racers.push_back(kart);
	}
	//Instantiate the CPU racers if necessary
	for (int i = human_players; i < toolbox->state.num_racers; i++) {
		int a = i % 4;
		int b = i / 4;
		Racer* kart = (Racer*)entity_manager->add_entity(new CPURacer(glm::vec3(
			track->start_pos.x * track->scale - a * 6, 
			track->start_pos.y * track->scale, 
			track->start_pos.z * track->scale - b*7), 
			toolbox, toolbox->camera, i,&racers)
		);
		kart->transform.scale *= 5.0f;
		if(i<8)
			kart->add_child(new DefaultEntity(glm::vec3(0, 0, 0), 
			toolbox->characters[CharacterType::ROBOT]->model)
		);
		racers.push_back(kart);
	}
}
GameScene::~GameScene() {
	track->unload();
}

void GameScene::update(float delta_time) {
	entity_manager->update(delta_time);
	gui_manager->update(delta_time);
	if(delta_time<0.1f)	//Leave some time for loading, otherwise can cause some jumps
		time += delta_time;

	if (intro) {
		float t = time;
		if (t > 2.32f) {	//Create a sine wave that syncs up with the start beeps 
							// 	to set the alpha value for the timer. Makes it pulse in time.
			countdown->colour.a = sin(t * 2.0f * M_PI - 2.0f);
			countdown->current_sprite = (int)((t - 2.32f));
		}
		if (t > 5.324f) {
			intro = false;
			//Play music and unfreeze racers
			toolbox->sound_manager->play_music(track->music, track->loop_point, 0.6f);
			for (int i = 0; i < racers.size(); i++) {
				racers[i]->frozen = false;
			}
			countdown->colour.a = 0;
		}

	}

	//Code for interracer collisions
	for (int i = 0; i < racers.size(); i++) {
		for (int j = i; j < racers.size(); j++) {
			if (i == j) continue;	//Cant collide with yourself
			if (glm::distance(racers[i]->transform.position, racers[j]->transform.position) < 3.0f) {	//Within 3 units?
				//Get the midpoint, get a directional vector to both players and apply velocity in those directions.
				glm::vec3 midpoint = (racers[i]->transform.position + racers[j]->transform.position) / 2.0f;

				glm::vec3 dir1 = glm::normalize(midpoint- racers[i]->transform.position);
				glm::vec3 dir2 = glm::normalize(midpoint - racers[j]->transform.position);

				//Only apply velocity if they haven't been hit recently to avoid launching them at higher framerates
				if (time - racers[i]->last_hit > 0.2f) {
					racers[i]->last_hit = time;
					racers[i]->hit_velocity = -dir1 * 20.0f;
				}
				if (time - racers[j]->last_hit > 0.2f) {
					racers[j]->last_hit = time;
					racers[j]->hit_velocity = -dir2 * 20.0f;
				}

			}
		}
		
		Racer* r = racers[i];
		if (r->finished) continue;

		//Checking if they are in a checkpoint bounding box
		BoundingBox c = track->checkpoints[(r->current_checkpoint + 1)%track->checkpoints.size()];
		float dist = glm::distance(c.position,r->transform.position);
		r->checkpoint_distance = dist;
		glm::vec3 centre = c.position;

		glm::vec3 E = c.scale;
		//Ensure that it touches the track
		E.y *= 2.0f;

		glm::vec3 local = r->transform.position - centre;
		//Rotation matrix to unrotate the player into box's coordinate space
		glm::mat4 rot = glm::rotate(glm::mat4(1.0f), -c.rot_y, glm::vec3(0, 1, 0));
		local = glm::vec3(rot * glm::vec4(local, 1.0f));
		//Check if the player is within the bounding box
		bool inside = (fabs(local.x) <= E.x) &&
			(fabs(local.y) <= E.y) &&
			(fabs(local.z) <= E.z);

		if (inside) {
			//Increment checkpoint
			r->current_checkpoint = (r->current_checkpoint + 1) % track->checkpoints.size();
			if (r->current_checkpoint == 0) {	//Was a finish line?
				if (r->current_lap == 2) {
					toolbox->state.places[numFinished] = r->place;
					r->finished = true;
					r->final_place = numFinished;
					numFinished++;
					if (numFinished == toolbox->state.num_racers) {	//Everyone finished? change to podium scene
						for (int r2 = 0; r2 < racers.size(); r2++) {
							toolbox->state.scores[r2] += 16 - racers[r2]->final_place * 2;
						}
						if(toolbox->settings.tournament)
							change_scene(new LeaderboardScene(gui_manager, entity_manager, toolbox),
							 toolbox->transitions["swipe"]
							);
						else
							change_scene(new PodiumScene(gui_manager, entity_manager, toolbox),
								toolbox->transitions["swipe"]
							);
						toolbox->sound_manager->soloud.stopAll();
						return;
					}
				}
				else	//Didn't finish, just new lap. Increment lap counter
					r->current_lap++;
			}
			r->checkpoint_distance = glm::distance(
				track->checkpoints[(r->current_checkpoint + 1) % track->checkpoints.size()].position, 
				r->transform.position
			);

		}
		
	}

	std::vector<int> places(racers.size());
	for (int a = 0; a < racers.size(); a++) {
		places[a] = a;
	}

	std::sort(places.begin(), places.end(), [this](int a, int b) {	//Sort the racers by their place
		Racer* a1 = racers[a];
		Racer* b1 = racers[b];
		if (a1->finished) {
			if (b1->finished) return a1->final_place < b1->final_place;
			return true;
		}
		else if (b1->finished) return false;
		if (a1->current_lap != b1->current_lap)
			return a1->current_lap > b1->current_lap;

		if (a1->current_checkpoint != b1->current_checkpoint)
			return a1->current_checkpoint > b1->current_checkpoint;

		return a1->checkpoint_distance < b1->checkpoint_distance;

		});
	for (int i = 0; i < places.size(); i++) {
		racers[places[i]]->current_place = i;
	}
}

//Render the screen for a specific racer.
void GameScene::render_screen(float x, float y, float w, float h, int r) {
	Racer* racer = racers[r];
	//Change where the screen is being drawn
	glViewport(x, y, w, h);
	
	//Set the camera position and rotation to that of the current racer
	toolbox->camera->transform.position = racer->camera_smooth_pos;
	toolbox->camera->transform.rotation = racer->camera_pos->transform.world_rotation;
	//Dynamic FoV to make the racing feel more immersive
	toolbox->camera->fov = 70.0f + glm::length(racer->velocity) * 1.5f;
	//Update the aspect ratio (useful only when there are 2 players)
	toolbox->camera->aspect = w / h;

	toolbox->camera->update_camera();

	//Turn off depth testing
	glDisable(GL_DEPTH_TEST);
	//Bind and render the skybox
	sky_shader->use();
	sky_shader->upload_uniforms();
	track->sky->bind(GL_TEXTURE0);
	sky->bind();
	sky->render();
	glEnable(GL_DEPTH_TEST);

	entity_manager->render();
	if (r % 2 == 0) { place->position.x = -8; lap->position.x = -12;}
	else { place->position.x = 8; lap->position.x = 12; }

	if (toolbox->state.num_players == 2) {
		gui_manager->aspect = ASPECT / 2.0f;
		place->position.x *= 1.5f;
		lap->position.x *= 1.5f;
	}
	gui_manager->setup_render_batch();
	place->current_sprite = racer->current_place;
	if (racer->finished) place->current_sprite = racer->final_place;
	lap->current_sprite = racer->current_lap;
	if(!racer->finished)
		gui_manager->render_one(lap);

	gui_manager->render_one(place);
	
	gui_manager->end_render_batch();
	gui_manager->aspect = ASPECT;
	
	if (racer->respawning) {	//Bind and render a square with the respawn shader
		respawn_transition->shader->use();
		respawn_transition->shader->blend = racer->respawn_blend;
		respawn_transition->shader->time = time;
		respawn_transition->shader->blend_full = racer->respawn_blend_full;
		respawn_transition->shader->upload_uniforms();
		glBindVertexArray(toolbox->square_vao);
		glEnableVertexAttribArray(0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	

}
void GameScene::render() {
	//Splitscreen stuff
	switch (toolbox->state.num_players) {
	case 1:	//1 Player
		render_screen(0, 0, WIDTH, HEIGHT, 0);
		break;
	case 2:	//2 Players
		render_screen(0, 0, WIDTH, HEIGHT / 2.0f, 1);
		render_screen(0, HEIGHT / 2.0f * 1, WIDTH, HEIGHT / 2.0f, 0);
		break;
	case 3:	//3 Players
		render_screen(0, 0, WIDTH / 2.0f, HEIGHT / 2.0f, 2);
		render_screen(WIDTH / 2.0f * 1, HEIGHT / 2.0f, WIDTH / 2.0f, HEIGHT / 2.0f, 1);
		render_screen(0, HEIGHT / 2.0f, WIDTH / 2.0f, HEIGHT / 2.0f, 0);
		//TODO: Fill blank space with something interesting
		break;
	case 4:	//4 Players
		render_screen(0, HEIGHT / 2.0f, WIDTH / 2.0f, HEIGHT / 2.0f, 0);
		render_screen(WIDTH / 2.0f, HEIGHT / 2.0f, WIDTH / 2.0f, HEIGHT / 2.0f, 1);
		render_screen(0, 0, WIDTH / 2.0f, HEIGHT / 2.0f, 2);
		render_screen(WIDTH / 2.0f * 1, 0, WIDTH / 2.0f, HEIGHT / 2.0f, 3);
		break;
	}
	glViewport(0, 0, WIDTH, HEIGHT);
	gui_manager->render();
	if(toolbox->state.num_players>1)
		glViewport(WIDTH / 2.0f - WIDTH*(0.3f+track->map_pos.x), 
			HEIGHT / 2.0f - HEIGHT * (0.3f + track->map_pos.y), 
			WIDTH * 0.6f, HEIGHT * 0.6f
		);
	else 
		glViewport(
			WIDTH * (0.4f + track->map_pos.x), 
			HEIGHT * (-0.15f + track->map_pos.y),
			WIDTH * 0.6f, HEIGHT * 0.6f
		);

	toolbox->camera->transform.position = glm::vec3(0, 5, 0);
	toolbox->camera->transform.set_rotation_degrees(glm::vec3(270, 180, 0));

	toolbox->camera->update_camera();
	
	toolbox->camera->set_orthographic(); 
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	square_texture->bind(GL_TEXTURE0);
	//Render the map object
	entity_manager->render_single_entity(track_map);
	glDisable(GL_BLEND);
	toolbox->camera->update_camera();

	//Render each player as a GUI object
	gui_manager->setup_render_batch();
	for (int i = 0; i < racers.size(); i++) {
		car_map->current_sprite = (racers[i]->human ? i : 4);
		//Set the position to the XZ of the racer
		car_map->position = glm::vec2(-racers[i]->transform.position.x,
			racers[i]->transform.position.z)/(track->scale*4.5f);
		//Render!
		gui_manager->render_one(car_map);
	}
	gui_manager->end_render_batch();

	glViewport(0, 0, WIDTH, HEIGHT);

	gui_manager->late_render();
}