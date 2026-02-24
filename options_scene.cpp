#include "options_scene.h"
#include "gui_image.h"
#include "character_scene.h"
#include "default_entity.h"
#include "menu_scene.h"
#include "gui_text.h"

OptionsScene::OptionsScene(GUIManager* gui, EntityManager* entities, Toolbox* toolbox) : Scene(gui, entities, toolbox)
{
}
void OptionsScene::start() {
	toolbox->sound_manager->play_music(toolbox->music[2], 1.714, 1.3f);
	//Cursors needed in the menu
	gui_manager->show_cursors = true;
	//Makes 3D elements in the menu more flat
	toolbox->camera->fov = 45.0f;
	//The top bar where the scene title will be
	GUIImage* image = new GUIImage(toolbox->gui["top_bar"],
		glm::vec2(0, 7.0f),
		glm::vec2(WIDTH_UNITS, 0.8f));

	gui_manager->add_object(image);

	back_button = (GUIButton*)gui_manager->add_object(
		new GUIButton(toolbox->gui["back_button"], glm::vec2(10.5f, -5.0f),
			glm::vec2(1.0f, 1.0f) * 1.5f,
			glm::vec4(1.05f),
			glm::vec4(1.2f, 1.2f, 1.2f, 1.0f), "", nullptr, 0.8f,
			glm::vec4(0.847f, 0.431f, 0.8f, 1.0f)));

	//Spinning logo
	logo = entity_manager->add_entity(
		new DefaultEntity(glm::vec3(-1.8f, -0.7f, -4.0f),
			toolbox->renderables["cog"]));

	logo->transform.scale *= 0.6f;

	gui_manager->add_object(new GUIText(glm::vec2(6, 7), 0.75f, toolbox->fonts["bold"], "OPTIONS"))->colour = glm::vec4(0.847f, 0.431f, 0.8f, 1.0f);
	gui_manager->add_object(new GUIImage(toolbox->gui["cogholder"],glm::vec2(-8.8f, -4), glm::vec2(5)));

	//Different settings

	master_slider = (GUIImage*)gui_manager->add_object(new GUIImage(toolbox->gui["slider"], glm::vec2(4, 3), glm::vec2(8,1)*0.8f));
	master_knob = (GUIImage*)gui_manager->add_object(new GUIImage(toolbox->gui["knob"], glm::vec2(4, 3), glm::vec2(1, 1) * 0.6f));
	master_text = (GUIText*)gui_manager->add_object(new GUIText(glm::vec2(2.3f, 4.4f), 0.75f, toolbox->fonts["bold"], "MASTER VOLUME: "));

	music_slider = (GUIImage*)gui_manager->add_object(new GUIImage(toolbox->gui["slider"], glm::vec2(4, 0), glm::vec2(8, 1) * 0.8f));
	music_knob = (GUIImage*)gui_manager->add_object(new GUIImage(toolbox->gui["knob"], glm::vec2(4, 0), glm::vec2(1, 1) * 0.6f));
	music_text = (GUIText*)gui_manager->add_object(new GUIText(glm::vec2(2.3f, 1.2f), 0.75f, toolbox->fonts["bold"], "MUSIC VOLUME: "));


	cpu_button = (GUIButton*)gui_manager->add_object(
		new GUIButton(toolbox->gui["main/player_button"], glm::vec2(2.8f, -3.0f),
			glm::vec2(5.0f, 1.0f),
			glm::vec4(1.05f),
			glm::vec4(1.2f, 1.2f, 1.2f, 1.0f), "CPU RACERS: ON", toolbox->fonts["bold"], 0.8f,
			glm::vec4(0.847f, 0.431f, 0.8f, 1.0f)));

	tournament_button = (GUIButton*)gui_manager->add_object(
		new GUIButton(toolbox->gui["main/player_button"], glm::vec2(2, -5.0f),
			glm::vec2(5.0f, 1.0f),
			glm::vec4(1.05f),
			glm::vec4(1.2f, 1.2f, 1.2f, 1.0f), "TOURNAMENT: ON", toolbox->fonts["bold"], 0.8f,
			glm::vec4(0.847f, 0.431f, 0.8f, 1.0f)));

	refresh();
}
OptionsScene::~OptionsScene() {

}

void OptionsScene::update(float delta_time) {
	//Spin the logo
	logo->transform.rotate(glm::vec3(0, delta_time * 50.0f, 0.0f));

	entity_manager->update(delta_time);
	gui_manager->update(delta_time);


	//Check for button presses
	if ((back_button->hovering & 1u) > 0) {
		if (toolbox->controllers[0]->buttons_pressed[0]) {
			toolbox->settings.write();
			toolbox->sound_manager->play_sound(toolbox->ui_sounds[3]);
			change_scene(new MenuScene(gui_manager, entity_manager, toolbox, true), toolbox->transitions["pixel"]);
		}
	}

	if ((cpu_button->hovering & 1u) > 0) {
		if (toolbox->controllers[0]->buttons_pressed[0]) {
			toolbox->settings.cpus = !toolbox->settings.cpus;
			refresh();
		}
	}
	if ((tournament_button->hovering & 1u) > 0) {
		if (toolbox->controllers[0]->buttons_pressed[0]) {
			toolbox->settings.tournament = !toolbox->settings.tournament;
			refresh();
		}
	}
	//Is the user pressing the slider? move it and change the volume
	if ((master_slider->hovering & 1u) > 0) {
		float relative = -((master_slider->position.x+1.3f)-toolbox->controllers[0]->cursor_position.x)/master_slider->scale.x;
		relative *= 1.1f;
		if (abs(relative)<1.0f && toolbox->controllers[0]->buttons[0]) {
			float fvol = max(-1.0f, min(1.0f, relative));
			float vol = (fvol + 1.0f) / 2.0f;
			toolbox->settings.master_volume = vol;
			toolbox->settings.update();
			refresh();
		}
	}
	if ((music_slider->hovering & 1u) > 0) {
		float relative = -((music_slider->position.x + 1.3f) - toolbox->controllers[0]->cursor_position.x) / music_slider->scale.x;
		relative *= 1.1f;
		if (abs(relative) < 1.0f && toolbox->controllers[0]->buttons[0]) {
			float fvol = max(-1.0f, min(1.0f, relative));
			float vol = (fvol + 1.0f) / 2.0f;
			toolbox->settings.music_volume = vol;
			toolbox->settings.update();
			refresh();
		}
	}
}
//Update looks of the settings according to what they are set to
void OptionsScene::refresh() {
	float vol = toolbox->settings.master_volume;
	master_knob->position.x = master_slider->position.x+((vol-0.5f)*1.8f)*master_slider->scale.x;
	int num = vol * 100.0f;
	master_text->text = "MASTER VOLUME: " + std::to_string(num);
	master_text->refresh();

	vol = toolbox->settings.music_volume;
	music_knob->position.x = music_slider->position.x + ((vol - 0.5f) * 1.8f) * music_slider->scale.x;
	num = vol * 100.0f;
	music_text->text = "MUSIC VOLUME: " + std::to_string(num);
	music_text->refresh();

	cpu_button->set_text("CPU RACERS: " + std::string(toolbox->settings.cpus ? "ON" : "OFF"));
	tournament_button->set_text("TOURNAMENT: " + std::string(toolbox->settings.tournament ? "ON" : "OFF"));
}

void OptionsScene::render() {
	gui_manager->render();
	entity_manager->render();

	gui_manager->late_render();
}