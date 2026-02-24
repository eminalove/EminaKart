#include "loading_scene.h"
#include "menu_scene.h"
#include "podium_scene.h"
#include "leaderboard_scene.h"
#include <algorithm>

LoadingScene::LoadingScene(GUIManager* gui, EntityManager* entities, Toolbox* toolbox) : Scene(gui, entities, toolbox) {

}

LoadingScene::~LoadingScene() {

}
void LoadingScene::start() {
	//Dont show cursors here
	gui_manager->show_cursors = false;	

	//Add loading text and a loading bar
	loading_status = (GUIText*)gui_manager->add_object(new GUIText(glm::vec2(-9.2f, 1.7f), 1, toolbox->fonts["bold"], "LOADING..."));
	loading_status->colour = glm::vec4(0.847f, 0.4313f, 0.8f, 1.0f);
	gui_manager->add_object(new GUIImage(toolbox->gui["square"], glm::vec2(0, 0), glm::vec2(10, 1)))->colour = glm::vec4(0.847f, 0.4313f, 0.8f, 1.0f);
	loading_bar = (GUIImage*)gui_manager->add_object(new GUIImage(toolbox->gui["square"], glm::vec2(-9.8, 0), glm::vec2(0, 0.8)));
	loading_bar->colour = glm::vec4(1.0f, 0.8235f, 0.9843f, 1.0f);
	update_time = 0.0f;
}
void LoadingScene::update(float delta_time) {
	gui_manager->update(delta_time);
	if (should_load) {
		//See what the current loading state is, and load that stage
		switch (update_iteration) {
		case 0:
			toolbox->load_textures();
			loading_status->text = "LOADING TEXTURES";
			loading_status->refresh();
			break;
		case 1:
			toolbox->load_shaders();
			loading_status->text = "LOADING SHADERS";
			loading_status->refresh();
			break;
		case 2:
			toolbox->load_particle_templates();
			loading_status->text = "LOADING PARTICLES";
			loading_status->refresh();
			break;
		case 3:
			toolbox->load_models();
			loading_status->text = "LOADING MODELS";
			loading_status->refresh();
			break;
		case 4:
			toolbox->load_gui_elements();
			loading_status->text = "LOADING GUI ELEMENTS";
			loading_status->refresh();
			break;
		case 5:
			toolbox->load_renderables();
			loading_status->text = "LOADING COMPLEX MODELS";
			loading_status->refresh();
			break;
		case 6:
			toolbox->load_fonts();
			loading_status->text = "LOADING FONTS";
			loading_status->refresh();
			break;
		case 7:
			toolbox->load_transitions();
			loading_status->text = "LOADING TRANSITIONS";
			loading_status->refresh();
			break;
		case 8:
			toolbox->load_characters();
			loading_status->text = "LOADING CHARACTERS";
			loading_status->refresh();
			break;
		case 9:
			toolbox->load_sounds();
			loading_status->text = "LOADING SOUNDS";
			loading_status->refresh();
			break;
		case 10:
			toolbox->load_tracks();
			loading_status->text = "LOADING TRACKS";
			loading_status->refresh();
			break;
		case 11:
			loading_status->text = "FINISHED LOADING :)";
			loading_status->refresh();
			finished_updating = true;
			toolbox->sound_manager->play_sound(toolbox->ui_sounds[2]);
			//Finished loading, load new scene
			change_scene(new MenuScene(gui_manager, entity_manager, toolbox,true), toolbox->transitions["pixel"]);
			break;
		}
		should_load = false;
		update_time = 0.0f;
	}
	if (!finished_updating) {
		//Interpolate between loading stages
		update_time += delta_time;
		if (update_time > 0.3f) {
			should_load = true;
			update_time = 0.0f;
			update_iteration++;
			current_scale = target_scale;
			target_scale = (update_iteration+1) / 11.0f * 9.8f;
		}
		//Cosine interpolation for some smooth non linear movement
		float t = update_time / 0.3f;
		float y = (1 - cos(t * M_PI)) / 2;
		loading_bar->scale.x = (current_scale * (1 - y) + target_scale * y);
		loading_bar->position.x = -9.8 + loading_bar->scale.x;
	}
	
	
}

void LoadingScene::render() {
	gui_manager->render();
	gui_manager->late_render();
}