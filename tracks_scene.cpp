#include "tracks_scene.h"
#include "character_scene.h"
#include "game_scene.h"

TracksScene::TracksScene(GUIManager* gui, EntityManager* entities, Toolbox* toolbox)
 : Scene(gui, entities, toolbox) {}

TracksScene::~TracksScene() {}

void TracksScene::start() {
	//Top bar and text
	gui_manager->add_object(new GUIImage(toolbox->gui["top_bar"],
		glm::vec2(0, 7.0f),
		glm::vec2(WIDTH_UNITS, 0.8f)));
	gui_manager->add_object(new GUIText(glm::vec2(6, 7), 0.7f,
	 toolbox->fonts["bold"], "TRACK SELECTION"))->colour = 
	 	glm::vec4(0.847f, 0.431f, 0.8f, 1.0f);


	//Simple buttons for back and start
	back_button = (GUIButton*)gui_manager->add_object(
		new GUIButton(toolbox->gui["back_button"], glm::vec2(11.5f, -6.0f),
			glm::vec2(1.0f, 1.0f) * 1.5f,
			glm::vec4(1.05f),
			glm::vec4(1.2f, 1.2f, 1.2f, 1.0f), "", nullptr, 0.8f,
			glm::vec4(0.847f, 0.431f, 0.8f, 1.0f)));

	
	start_button = (GUIButton*)gui_manager->add_object(
		new GUIButton(toolbox->gui["main/one_player"], 
			glm::vec2(8.5f, 4.6f) - glm::vec2(0.5f, 1.1f) * 8.3f,
			glm::vec2(5.5f, 2.456f) * 0.93f,
			glm::vec4(1),
			glm::vec4(1.2f, 1.2f, 1.2f, 1.0f), "  GO!", toolbox->fonts["bold"], 2,
			glm::vec4(0.847f, 0.431f, 0.8f, 1.0f)));
	
	//Track preview image along with its name
	track_image = (GUIImage*)gui_manager->add_object(
		new GUIImage(toolbox->tracks[selected_track]->preview, glm::vec2(-6, 1),
		 glm::vec2(1.3333f,1.0f)*4.5f));

	track_name = (GUIButton*)gui_manager->add_object(
		new GUIButton(toolbox->gui["character/player_box"], glm::vec2(8.5f, 4.6f),
			glm::vec2(5.5f, 1.0f) * 0.8f,
			glm::vec4(1.05f),
			glm::vec4(1.2f, 1.2f, 1.2f, 1.0f), toolbox->tracks[selected_track]->name,
			toolbox->fonts["bold"], 0.8f,
			glm::vec4(1.0f)));

}

void TracksScene::update(float delta_time) {

	entity_manager->update(delta_time);
	gui_manager->update(delta_time);

	//Check for buttons being pressed
	if (back_button->hovering > 0 && toolbox->controllers[0]->buttons_pressed[0]) {
		toolbox->sound_manager->play_sound(toolbox->ui_sounds[3]);
		change_scene(new CharacterScene(
			gui_manager, entity_manager, toolbox), toolbox->transitions["pixel"]);
	}

	
	if (start_button->hovering > 0 && toolbox->controllers[0]->buttons_pressed[0]) {
		toolbox->sound_manager->play_sound(toolbox->ui_sounds[2]);
		toolbox->sound_manager->stop_music(true);
		toolbox->state.current_track = toolbox->tracks[selected_track];
		change_scene(new GameScene(
			gui_manager, entity_manager, toolbox), toolbox->transitions["swipe"]);
	}
	//If the track name is pressed, cycle through the tracks
	if (track_name->hovering > 0 && toolbox->controllers[0]->buttons_pressed[0]) {
		toolbox->sound_manager->play_sound(toolbox->ui_sounds[0]);
		selected_track = (selected_track + 1) % toolbox->tracks.size();
		track_name->set_text(toolbox->tracks[selected_track]->name);
		track_image->texture = toolbox->tracks[selected_track]->preview;
	}
}

void TracksScene::render() {
	entity_manager->render();
	gui_manager->render();

	gui_manager->late_render();
}