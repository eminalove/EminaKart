#include "racer.h"
#include "textured_model.h"
#include "default_entity.h"
#include "particle_shader.h"
#include "empty_entity.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/constants.hpp>

Racer::Racer(glm::vec3 position, Toolbox* toolbox, Camera* camera, int place, bool human) {
	sound = toolbox->sound_manager;
	this->human = human;
	start_pos = position;
	true_pos = position;
	this->toolbox = toolbox;
	this->place = place;
	time = 0;
	respawn_time = 0;
	leave_ground_time = 0;
	if (human)
		engine_sound = sound->soloud.play(*toolbox->sounds[0]);
	sound->soloud.setLooping(engine_sound, true);
	pan = 0.0f;
	if (toolbox->state.num_players > 2) {
		pan = place % 2 ? 0.5f : -0.5f;
	}
	sound->soloud.setPan(engine_sound, pan);
	this->transform.position = position;
	if (place % 4 == 0) model = toolbox->renderables["kart1"];
	else if (place % 4 == 1) model = toolbox->renderables["kart1skin2"];
	else if (place % 4 == 2) model = toolbox->renderables["kart1skin3"];
	else if (place % 4 == 3) model = toolbox->renderables["kart1skin4"];
	this->transform.scale = glm::vec3(1);
	this->acceleration = 0.0f;
	this->velocity = glm::vec3(0);
	rot_y = 0;
	hit_velocity = glm::vec3(0);

	this->track = toolbox->state.current_track;

	//Add 4 wheels and position and rotate them correctly
	Renderable* wheel_model = toolbox->renderables["wheel"];
	add_child(new DefaultEntity(glm::vec3(-0.26f, 0, -0.42f), wheel_model));
	add_child(new DefaultEntity(glm::vec3(-0.23f, 0.015f, 0.2f), wheel_model))->transform.scale *= 1.1f;

	add_child(new DefaultEntity(glm::vec3(0.26f, 0, -0.42f), wheel_model))->transform.rotate(glm::vec3(0,180,0));
	Entity* e = add_child(new DefaultEntity(glm::vec3(0.23f, 0.015f, 0.2f), wheel_model));
	e->transform.scale *= 1.1f;
	e->transform.rotate(glm::vec3(0, 180, 0));
	last_track_normal = glm::vec3(0, 1, 0);
	//Add the steering wheel and put it in place
	Entity* s = add_child(new DefaultEntity(glm::vec3(0.0f, 0.15f, -0.29f), toolbox->renderables["steeringwheel"]));
	s->transform.rotate(glm::vec3(30.0f, 0.0f, 0.0f));

	p1 = new ParticleSystem(glm::vec3(-0.1f, 0.2f, 0.3f), toolbox->particle_templates["exhaust_smoke"], 
		(ParticleShader*)toolbox->shaders["particle"], toolbox->square_vao, camera);
	p2 = new ParticleSystem(glm::vec3(0.1f, 0.2f, 0.3f), toolbox->particle_templates["exhaust_smoke"], 
		(ParticleShader*)toolbox->shaders["particle"], toolbox->square_vao, camera);
	add_child(p1);
	add_child(p2);

	camera_pos = add_child(new EmptyEntity(glm::vec3(0, 0.8f, 1.2f)));
	camera_pos->transform.rotate(glm::vec3(-15, 0, 0));
}
Racer::~Racer() {
	std::cout << "Stop sound\n";
	toolbox->sound_manager->soloud.stop(engine_sound);
}
void Racer::update(float delta_time) {
	delta_time *= 2.0f;
	camera_smooth_pos = glm::lerp(camera_smooth_pos, camera_pos->transform.world_position, delta_time * 10.0f);

	camera_smooth_rot = glm::lerp(camera_smooth_rot, camera_pos->transform.world_rotation, glm::clamp(delta_time * 10.0f,0.0f,1.0f));
	
	//Give some acceleration to the stick to make it feel less snappy
	float delta_stick = dampened_left_stick - left_stick;
	dampened_left_stick -= delta_stick * delta_time*(4.0f);

	float volMod = (glm::min)(glm::length(velocity),0.7f)+0.3f;
	time += delta_time;
	
	sound->soloud.setVolume(engine_sound, volMod);
	sound->soloud.setRelativePlaySpeed(engine_sound, glm::length(velocity)/20.0f + 1.0f);

	//Accelerate and turn on the smoke if the player presses a button
	p1->emitting = false;
	p2->emitting = false;
	if (should_accelerate) {
		if(acceleration<5.0f)	//Under terminal acceleration? Accelerate
			acceleration = (glm::min)(5.0f,acceleration+delta_time);
		else {					//If not, slow down until you are
			acceleration = (glm::max)(0.0f, acceleration - delta_time * 1.7f);
		}						//Should emit smoke if accelerating
		p1->emitting = true;
		p2->emitting = true;
	}
	else {
		//Apply friction
		acceleration= (glm::max)(0.0f, acceleration - delta_time*1.7f);
	}

	float friction = 1.2f;

	bool drifting = should_drift;

	//Accelerate forward
	glm::vec3 forward = glm::vec3(cos(glm::radians(rot_y - 90)), 0, sin(glm::radians(rot_y - 90)));
	velocity += acceleration * delta_time * -5.0f * forward * friction * (!human ? 1.03f : 1.0f) /** (1.0f+(current_place/25.0f))*/;
	


	//How strong rotation should be based on speed
	float rot_coefficient = glm::length(velocity) > 8 ? 1 : length(velocity)/8.0f;

	//Rotate using the left stick
	rot_y+= dampened_left_stick *delta_time*70.0f * rot_coefficient;

	//Rotate the steering wheel
	children[4]->transform.set_rotation_degrees(glm::vec3(30.0f, dampened_left_stick * -45.0f, 0.0f));

	//Move with the velocity
	if (drifting) {

		true_pos += velocity * delta_time;
	}
	else {
		true_pos += glm::length(velocity) * -1.0f * forward * delta_time;
	}

	//Apply friction
	velocity *= exp(-friction * delta_time);
	hit_velocity *= exp(-friction * delta_time*2.3f);

	
	//Rotate the wheels with the speed of the car
	rotate_wheels(-50.0f*glm::length(velocity)*delta_time, dampened_left_stick * -45.0f);
	Triangle tri;
	
	glm::vec3 hit_pos, hit_norm;
	
	//Get the distance from the track
	float dist = track->intersectRay({ true_pos + glm::vec3(0,2.5,0),-last_track_normal }, tri, hit_pos, hit_norm);
	if (tri.material == 1) {
		//boost
		acceleration = 10.0f;
	}
	true_pos += hit_velocity*delta_time;

	//Close enough to the track?
	if (dist > 0 && dist <5) {
		//Move to be 0.8 units above the track
		true_pos += (0.8f-dist)*last_track_normal;
		//No gravity while grounded
		y_vel = 0;

		//Gives the negative of the next downwards vector
		last_track_normal = hit_norm;
		//Reset the time when the 
		// racer left the track each frame
		leave_ground_time = time;
	}
	else {
		//Move downwards each frame with gravity
		y_vel += delta_time/30.0f;
		true_pos -= y_vel*last_track_normal;
	}

	//Right vector -->
	glm::vec3 right = glm::normalize(
		glm::cross(last_track_normal, forward));

	//Forward vector (can't ascii that one)
	forward = glm::normalize(glm::cross(right, last_track_normal));

	//Get the rotation matrix for the normal and the user rotation
	glm::mat3 rotation(right, last_track_normal, forward);

	//Convert this to a quaternion and set it to the rotation
	glm::quat orientation = glm::quat_cast(rotation);
	transform.rotation = orientation;

	//Has the player fallen off?
	if (!respawning && (time-respawn_time>5.0f) && time - leave_ground_time>1.5f) {
		respawning = true;
		respawn_blend_full = 0.0f;
		respawn_blend = 0.0f;
		
		//Play the fall sound
		if(human)
			sound->play_sound(toolbox->sounds[1],0.4f,pan);
	}
	if (respawning) {
		if (respawn_blend_full < 1.0f) {
			respawn_blend_full += delta_time;
			respawn_blend += delta_time;
			if (respawn_blend_full >= 1.0f) {
				//Reset everything
				leave_ground_time = time;
				//Reset position
				true_pos = track->checkpoints[current_checkpoint].position;
				respawn_time = time;
				acceleration = 0.0f;
				rot_y = -glm::degrees(track->checkpoints[current_checkpoint].rot_y);
				last_track_normal = glm::vec3(0, 1, 0);

				transform.rotation = glm::quat();
				velocity = glm::vec3(0);
				hit_velocity = glm::vec3(0);
			}
		}
		else {
			respawn_blend -= delta_time;
			respawn_blend_full += delta_time;
			if (respawn_blend_full >= 2.0f) {
				respawning = false;
			}
		}
	}
	transform.world_position = true_pos + glm::vec3(0, -1.6f, 0);
	transform.position = true_pos + glm::vec3(0, -1.6f, 0);
	//transform.set_rotation_degrees(glm::vec3(align_road.x, rot_y, align_road.z));
	
}
void Racer::rotate_wheels(float amount, float turn) {
	children[0]->transform.rotate(glm::vec3(amount, 0, 0));
	children[1]->transform.rotate(glm::vec3(amount, 0, 0));
	children[2]->transform.rotate(glm::vec3(amount, 0, 0));
	children[3]->transform.rotate(glm::vec3(amount, 0, 0));

	glm::vec3 rot = children[0]->transform.get_euler_degrees();
	children[0]->transform.set_rotation_degrees(glm::vec3(rot.x, dampened_left_stick * -25.0f, 0));
	rot = children[2]->transform.get_euler_degrees();
	children[2]->transform.set_rotation_degrees(glm::vec3(rot.x, dampened_left_stick * 25.0f, 180));
}