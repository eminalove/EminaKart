#include "podium_scene.h"
#include "default_entity.h"
#include "menu_scene.h"

PodiumScene::PodiumScene(GUIManager* gui, EntityManager* entities, Toolbox* toolbox) 
	: Scene(gui, entities, toolbox) {
	
}

PodiumScene::~PodiumScene() {

}

void PodiumScene::start() {
	sky_shader = (SkyShader*)toolbox->shaders["sky"];
	//Create the podium and set its transform
	Entity* podium = entity_manager->add_entity(
		new DefaultEntity(glm::vec3(0, 0, -6), toolbox->renderables["podium"])
	);
	podium->transform.set_rotation_degrees(glm::vec3(0, 180, 0));
	toolbox->camera->fov = 90.0f;
	toolbox->camera->transform.position = glm::vec3(0, 0.8f, 0);
	toolbox->camera->transform.set_rotation_degrees(glm::vec3(0));
	entity_manager->add_entity(
		new DefaultEntity(glm::vec3(0, 0, -6),
		 toolbox->renderables["castle"])
	)->transform.set_rotation_degrees(glm::vec3(0, 180, 0));
	sky = new TexturedModel(toolbox->models["sky"], toolbox->skies["normal"]);

	//Play the triumphant music
	toolbox->sound_manager->play_music(toolbox->music[4],0.685f,0.6f);

	//Congratulations text
	congrats = (GUIImage*)gui_manager->add_object(new GUIImage(
		toolbox->gui["congratulations"], glm::vec2(0, -5), glm::vec2(10, 2.5))
	);

	//How many people should be celebrated?
	int numCelebrate = 3;
	if (!toolbox->settings.cpus) { //Less if there arent CPUs
		numCelebrate = max(3,toolbox->state.num_players);
	}
	for (int i = 0; i < numCelebrate; i++) {
		//First place is middle podium, second is first one and third last.
		int r = i == 0 ? 1 : (i == 1 ? 0 : 2);	
		//Is this racer a CPU?
		bool isCPU = toolbox->state.places[r] >= toolbox->state.num_players;
		//Only draw robots for CPUs
		int characterIndex = isCPU ? CharacterType::ROBOT : 
			toolbox->state.player_characters[toolbox->state.places[r]];
		Entity* character = entity_manager->add_entity(
			new DefaultEntity(glm::vec3(-3.0f+i*3.0f, 0, -7), 
			toolbox->characters[characterIndex]->model)
		);
		character->transform.scale *= 5.0f;
		//Make them face the camera
		character->transform.rotate(glm::vec3(0, 205-i*25, 0));
		character->transform.position.y = i == 0 ? 0.2f : (i == 1 ? 0.8f : -0.3f);
		chars.push_back(character);
	}
}

void PodiumScene::render() {
	//Turn off depth testing
	glDisable(GL_DEPTH_TEST);
	//Bind and render the skybox
	sky_shader->use();
	sky_shader->upload_uniforms();

	sky->bind();
	sky->render();
	glEnable(GL_DEPTH_TEST);

	entity_manager->render();
	gui_manager->render();
	
	gui_manager->late_render();
}

void PodiumScene::update(float delta_time) {
	time += delta_time;
	entity_manager->update(delta_time);
	//Make the characters bounce up and down celebrating
	for (int i = 0; i < chars.size(); i++) {
		chars[i]->transform.scale.y = (
			abs(sinf(time*3.0f + (i*0.1f*time)))
		)/4.0f + chars[i]->transform.scale.x;
	}
	//Make the congratulations text grow and fade in
	float t = max(0,min(time - 1.0f,1.0f));
	congrats->scale.y = t*2.5f;
	congrats->colour.a = t;
	if (time > 2.0f) {	//After 2 seconds let the user exit
		if (toolbox->controllers[0]->buttons_pressed[0]) {
			change_scene(new MenuScene(
				gui_manager,
				entity_manager, 
				toolbox, 
				true
			),toolbox->transitions["swipe"]);
		}
	}
}