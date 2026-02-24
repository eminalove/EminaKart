#include "menu_scene.h"
#include "gui_image.h"
#include "character_scene.h"
#include "default_entity.h"
#include "options_scene.h"
#include "gui_text.h"

MenuScene::MenuScene(GUIManager* gui, EntityManager* entities, Toolbox* toolbox, bool play_music) : Scene(gui,entities,toolbox)
{
	this->play_music = play_music;
}
void MenuScene::start() {
	if(play_music)
		toolbox->sound_manager->play_music(toolbox->music[0], 2.039,1.3f);
	//Cursors needed in the menu
	gui_manager->show_cursors = true;
	//Makes 3D elements in the menu more flat
	toolbox->camera->fov = 45.0f;
	//The top bar where the scene title will be
	GUIImage* image = new GUIImage(toolbox->gui["top_bar"],
		glm::vec2(0, 7.0f),
		glm::vec2(WIDTH_UNITS, 0.8f));

	gui_manager->add_object(image);
	
	two_button = (GUIButton*)gui_manager->add_object(
		new GUIButton(toolbox->gui["main/player_button"], glm::vec2(2.5f, 4.6f),
			glm::vec2(5.5f, 1.0f)*0.85f,
			glm::vec4(1.05f),
			glm::vec4(1.2f, 1.2f, 1.2f, 1.0f), "2 PLAYERS", toolbox->fonts["bold"],0.8f,
			glm::vec4(0.847f,0.431f,0.8f,1.0f)));
	three_button = (GUIButton*)gui_manager->add_object(
		new GUIButton(toolbox->gui["main/player_button"], glm::vec2(2.5f, 4.6f)-glm::vec2(0.5f,1.1f)*1.5f,
			glm::vec2(5.5f, 1.0f) * 0.85f,
			glm::vec4(1.05f),
			glm::vec4(1.2f, 1.2f, 1.2f, 1.0f), "3 PLAYERS", toolbox->fonts["bold"], 0.8f,
			glm::vec4(0.847f, 0.431f, 0.8f, 1.0f)));
	four_button = (GUIButton*)gui_manager->add_object(
		new GUIButton(toolbox->gui["main/player_button"], glm::vec2(2.5f, 4.6f) - glm::vec2(0.5f, 1.1f) * 3.0f,
			glm::vec2(5.5f, 1.0f) * 0.85f,
			glm::vec4(1.05f),
			glm::vec4(1.2f, 1.2f, 1.2f, 1.0f), "4 PLAYERS", toolbox->fonts["bold"], 0.8f,
			glm::vec4(0.847f, 0.431f, 0.8f, 1.0f)));
	start_button = (GUIButton*)gui_manager->add_object(
		new GUIButton(toolbox->gui["main/one_player"], glm::vec2(-8.0f, 3.0f),
			glm::vec2(5.5f, 2.5f),
			glm::vec4(1.05f),
			glm::vec4(1.2f, 1.2f, 1.2f, 1.0f), "  1 PLAYER", toolbox->fonts["bold"], 1.2f,
			glm::vec4(0.847f, 0.431f, 0.8f, 1.0f)));

	options_button = (GUIButton*)gui_manager->add_object(
		new GUIButton(toolbox->gui["main/settings"], glm::vec2(9.5f, 3.0f),
			glm::vec2(1.0f, 1.0f) * 1.5f,
			glm::vec4(1.05f),
			glm::vec4(1.2f, 1.2f, 1.2f, 1.0f), "", nullptr, 0.8f,
			glm::vec4(0.847f, 0.431f, 0.8f, 1.0f)));

	/*gui_manager->add_object(
		new GUIButton(toolbox->gui["main/stats"], glm::vec2(9.5f, -0.5f),
			glm::vec2(1.0f, 1.0f) * 1.5f,
			glm::vec4(1.05f),
			glm::vec4(1.2f, 1.2f, 1.2f, 1.0f), "", nullptr, 0.8f,
			glm::vec4(0.847f, 0.431f, 0.8f, 1.0f)));*/

	quit_button = (GUIButton*)gui_manager->add_object(
		new GUIButton(toolbox->gui["main/quit"], glm::vec2(9.5f, -4.0f),
			glm::vec2(1.0f, 1.0f) * 1.5f,
			glm::vec4(1.05f),
			glm::vec4(1.2f, 1.2f, 1.2f, 1.0f), "", nullptr, 0.8f,
			glm::vec4(0.847f, 0.431f, 0.8f, 1.0f)));

	//Spinning logo
	logo = entity_manager->add_entity(
		new DefaultEntity(glm::vec3(-1.2f, -0.7f, -4.0f),
			toolbox->renderables["logo"]));

	logo->transform.scale *= 0.8f;

	gui_manager->add_object(new GUIText(glm::vec2(6, 7), 0.75f, toolbox->fonts["bold"], "MAIN MENU"))->colour = glm::vec4(0.847f, 0.431f, 0.8f, 1.0f);


}
MenuScene::~MenuScene() {

}

void MenuScene::update(float delta_time) {
	//Spin the logo
	logo->transform.rotate(glm::vec3(0, delta_time*50.0f, 0));

	entity_manager->update(delta_time);
	gui_manager->update(delta_time);

	//Is the button pressed?
	if ((start_button->hovering & 1u) > 0) {
		if (toolbox->controllers[0]->buttons_pressed[0]) {
			toolbox->sound_manager->play_sound(toolbox->ui_sounds[0]);
			toolbox->state.num_players = 1;
			if (toolbox->settings.cpus) toolbox->state.num_racers = 8;
			else toolbox->state.num_racers = 1;
			
			//Change the scene to the game scene
			change_scene(new CharacterScene(
				gui_manager, entity_manager, toolbox),toolbox->transitions["pixel"]);
		}
	}
	if ((two_button->hovering & 1u) > 0) {
		if (toolbox->controllers[0]->buttons_pressed[0]) {
			toolbox->sound_manager->play_sound(toolbox->ui_sounds[0]);
			toolbox->state.num_players = 2;

			if (toolbox->settings.cpus) toolbox->state.num_racers = 8;
			else toolbox->state.num_racers = 2;
			//Change the scene to the game scene
			change_scene(new CharacterScene(
				gui_manager, entity_manager, toolbox), toolbox->transitions["pixel"]);
		}
	}
	if ((three_button->hovering & 1u) > 0) {
		if (toolbox->controllers[0]->buttons_pressed[0]) {
			toolbox->sound_manager->play_sound(toolbox->ui_sounds[0]);
			toolbox->state.num_players = 3;

			if (toolbox->settings.cpus) toolbox->state.num_racers = 8;
			else toolbox->state.num_racers = 3;
			//Change the scene to the game scene
			change_scene(new CharacterScene(
				gui_manager, entity_manager, toolbox), toolbox->transitions["pixel"]);
		}
	}
	if ((four_button->hovering & 1u) > 0) {
		if (toolbox->controllers[0]->buttons_pressed[0]) {
			toolbox->sound_manager->play_sound(toolbox->ui_sounds[0]);
			toolbox->state.num_players = 4;

			if (toolbox->settings.cpus) toolbox->state.num_racers = 8;
			else toolbox->state.num_racers = 4;
			//Change the scene to the game scene
			change_scene(new CharacterScene(
				gui_manager, entity_manager, toolbox), toolbox->transitions["pixel"]);
		}
	}
	if ((options_button->hovering & 1u) > 0) {
		if (toolbox->controllers[0]->buttons_pressed[0]) {
			toolbox->sound_manager->play_sound(toolbox->ui_sounds[0]);
			//Change the scene to the game scene
			change_scene(new OptionsScene(
				gui_manager, entity_manager, toolbox), toolbox->transitions["swipe"]);
		}
	}
	//Exit button, ends program
	if ((quit_button->hovering & 1u) > 0) {
		if (toolbox->controllers[0]->buttons_pressed[0]) {
			delete toolbox->sound_manager;
			glfwTerminate();
			exit(1);
		}
	}
}

void MenuScene::render() {
	entity_manager->render();
	gui_manager->render();

	gui_manager->late_render();
}