#include "character_scene.h"
#include "menu_scene.h"
#include "tracks_scene.h"
#include "game_scene.h"

CharacterScene::CharacterScene(GUIManager* gui, EntityManager* entities, Toolbox* toolbox) : Scene(gui, entities, toolbox) {}

CharacterScene::~CharacterScene() {}

void CharacterScene::start() {
	//Top bar with the scene title
	gui_manager->add_object(new GUIImage(toolbox->gui["top_bar"],
		glm::vec2(0, 7.0f),
		glm::vec2(WIDTH_UNITS, 0.8f))); 
	gui_manager->add_object(new GUIText(glm::vec2(6, 7), 0.7f, toolbox->fonts["bold"], "CHARACTER SELECTION"))->colour = glm::vec4(0.847f, 0.431f, 0.8f, 1.0f);
	
	//Button that returns to the main menu
	back_button = (GUIButton*)gui_manager->add_object(
		new GUIButton(toolbox->gui["back_button"], glm::vec2(11.5f, -6.0f),
			glm::vec2(1.0f, 1.0f) * 1.5f,
			glm::vec4(1.05f),
			glm::vec4(1.2f, 1.2f, 1.2f, 1.0f), "", nullptr, 0.8f,
			glm::vec4(0.847f, 0.431f, 0.8f, 1.0f)));

	
	//Player buttons 1-4, should only exist if there are enough players for it
	player_texts.push_back((GUIButton*)gui_manager->add_object(
		new GUIButton(toolbox->gui["character/player_box"], glm::vec2(8.5f, 4.6f),
			glm::vec2(5.5f, 1.0f) * 0.8f,
			glm::vec4(1.05f),
			glm::vec4(1.2f, 1.2f, 1.2f, 1.0f), "P1:         ", toolbox->fonts["bold"], 0.8f,
			glm::vec4(0.949f, 0.812f, 0.933f, 1.0f))));
	if(toolbox->state.num_players>1)
	player_texts.push_back((GUIButton*)gui_manager->add_object(
		new GUIButton(toolbox->gui["character/player_box"], glm::vec2(8.5f, 4.6f) - glm::vec2(0.5f, 1.1f) * 1.7f,
			glm::vec2(5.5f, 1.0f) * 0.8f,
			glm::vec4(1.05f),
			glm::vec4(1.2f, 1.2f, 1.2f, 1.0f), "P2:         ", toolbox->fonts["bold"], 0.8f,
			glm::vec4(0.949f, 0.812f, 0.933f, 1.0f))));
	if (toolbox->state.num_players > 2)
	player_texts.push_back((GUIButton*)gui_manager->add_object(
		new GUIButton(toolbox->gui["character/player_box"], glm::vec2(8.5f, 4.6f) - glm::vec2(0.5f, 1.1f) * 3.4f,
			glm::vec2(5.5f, 1.0f) * 0.8f,
			glm::vec4(1.05f),
			glm::vec4(1.2f, 1.2f, 1.2f, 1.0f), "P3:         ", toolbox->fonts["bold"], 0.8f,
			glm::vec4(0.949f, 0.812f, 0.933f, 1.0f))));
	if (toolbox->state.num_players > 3)
	player_texts.push_back((GUIButton*)gui_manager->add_object(
		new GUIButton(toolbox->gui["character/player_box"], glm::vec2(8.5f, 4.6f) - glm::vec2(0.5f, 1.1f) * 5.1f,
			glm::vec2(5.5f, 1.0f) * 0.8f,
			glm::vec4(1.05f),
			glm::vec4(1.2f, 1.2f, 1.2f, 1.0f), "P4:         ", toolbox->fonts["bold"], 0.8f,
			glm::vec4(0.949f, 0.812f, 0.933f, 1.0f))));

	//Start button- goes to track selection screen
	start_button = (GUIButton*)gui_manager->add_object(
		new GUIButton(toolbox->gui["main/one_player"], glm::vec2(8.5f, 4.6f) - glm::vec2(0.5f, 1.1f) * 8.3f,
			glm::vec2(5.5f, 2.456f) * 0.93f,
			glm::vec4(1),
			glm::vec4(1.2f, 1.2f, 1.2f, 1.0f), "  OK!", toolbox->fonts["bold"], 2,
			glm::vec4(0.847f, 0.431f, 0.8f, 1.0f)));
	
	//Create buttons for all the different characters
	int index = 0;
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < 3; i++) {
			//Get the current character
			Character* c = toolbox->characters[index++];

			//Create a button for it with the character texture index
			GUIButton* button = (GUIButton*)gui_manager->add_object(
				new GUIButton(toolbox->gui["character/characters"], glm::vec2(-10 + i*3.3f, 5 - j*3.0f),glm::vec2(1.5f),
					glm::vec4(1.0f),glm::vec4(1.05f),"",nullptr,0,glm::vec4(0.0f)));
			character_buttons.push_back(button);
			button->num_sprites = 12;
			button->current_sprite = c->type;
		}
	}

	//Selected character buttons
	for (int i = 0; i < toolbox->state.num_players; i++) {
		int c = toolbox->state.player_characters[i];
		if(c!=-1)
			player_texts[i]->set_text("P" + std::to_string(i + 1) + ": " + toolbox->characters[c]->short_name);
	}
	
}

void CharacterScene::update(float delta_time) {
	entity_manager->update(delta_time);
	gui_manager->update(delta_time);

	//If a player is hovering over the back button and player one clicks, go back.
	if (back_button->hovering > 0 && toolbox->controllers[0]->buttons_pressed[0]) {
		toolbox->sound_manager->play_sound(toolbox->ui_sounds[3]);
		change_scene(new MenuScene(
			gui_manager, entity_manager, toolbox), toolbox->transitions["pixel"]);
	}
	
	//Checks if a player is clicking a character button
	bool selected_all = true;
	for (int i = 0; i < toolbox->state.num_players; i++) {
		if (toolbox->state.player_characters[i] == -1) selected_all = false;
		GenericController* c = toolbox->controllers[i];
		//Go through each character button
		for (int j = 0; j < character_buttons.size(); j++) {
			GUIButton* button = character_buttons[j];
			//Is this player hovering over this button? bitwise operation to figure it out
			if ((button->hovering & (1u << i)) > 0) {
				if (c->buttons_pressed[0]) {
					//Update text and the picked character for this player
					player_texts[i]->set_text("P"+std::to_string(i+1) + ": " + toolbox->characters[j]->short_name);
					toolbox->state.player_characters[i] = j;
				}
			}
		}
	}
	//Set colour to dimmer if the button should be disabled, as not all players have selected a character
	start_button->colour = selected_all ? glm::vec4(1) : glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	start_button->hovering = selected_all ? start_button->hovering : 0;
	if (selected_all && start_button->hovering > 0 && toolbox->controllers[0]->buttons_pressed[0]) {
		toolbox->sound_manager->play_sound(toolbox->ui_sounds[1]);
		if (toolbox->settings.tournament) {
			toolbox->state.cTrack = 0;
			for (int i = 0; i < 8; i++) {
				toolbox->state.places[i] = i;
				toolbox->state.scores[i] = 0;
			}
			toolbox->state.current_track = toolbox->tracks[0];
			toolbox->sound_manager->stop_music(true);
			change_scene(new GameScene(
				gui_manager, entity_manager, toolbox), toolbox->transitions["swipe"]);
		}
		else {

			change_scene(new TracksScene(
				gui_manager, entity_manager, toolbox), toolbox->transitions["pixel"]);
		}
	}
}

//Simple render function
void CharacterScene::render() {
	entity_manager->render();
	gui_manager->render();

	gui_manager->late_render();
}