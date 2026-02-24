#include "leaderboard_scene.h"
#include "podium_scene.h"
#include "game_scene.h"

LeaderboardScene::LeaderboardScene(
	GUIManager* gui, EntityManager* entities, 
	Toolbox* toolbox) : Scene(gui, entities, toolbox)
{

}

LeaderboardScene::~LeaderboardScene() {

}

void LeaderboardScene::start() {
	gui_manager->add_object(new GUIImage(toolbox->gui["top_bar"],
		glm::vec2(0, 7.0f),
		glm::vec2(WIDTH_UNITS, 0.8f)));
	gui_manager->add_object(new GUIText(
		glm::vec2(6, 7), 0.7f, 
		toolbox->fonts["bold"], "LEADERBOARD")
	)->colour = glm::vec4(0.847f, 0.431f, 0.8f, 1.0f);

	int pIndex[8] = {0,1,2,3,4,5,6,7};

	//Sort players using a bubble sort
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8 - i - 1; j++) {
			if (toolbox->state.scores[pIndex[j]] 
				< toolbox->state.scores[pIndex[j + 1]]) {
				int temp = pIndex[j];
				pIndex[j] = pIndex[j + 1];
				pIndex[j + 1] = temp;
			}
		}
	}

	//Play the leaderboard music
	toolbox->sound_manager->play_music(toolbox->music[5], 0.685f, 0.6f);

	//Add all the players to the leaderboard list
	for (int a = 0; a < toolbox->state.num_racers; a++) {
		int i = pIndex[a];
		//Add the box that the text goes in
		leaderboard_items.push_back(gui_manager->add_object(
			new GUIImage(toolbox->gui["leaderboard_item"],
				glm::vec2(0), glm::vec2(15, 1) / 2.0f)
		));
		bool isCPU = i >= toolbox->state.num_players;

		//Set the text for the player or CPU
		GUIText* text2 = new GUIText(
			glm::vec2(0, 0), 0.7f, toolbox->fonts["bold"], 
			(isCPU ? ("CPU " + std::to_string(
				(i - toolbox->state.num_players)+1) + ":                          "
			)
				: ("PLAYER " + std::to_string(i + 1) + ":                       "))
					+std::to_string(toolbox->state.scores[i]));
		text2->colour = glm::vec4(0.847f, 0.431f, 0.8f, 1.0f);
		leaderboard_items.push_back(gui_manager->add_object(text2));
	}
	GUIText* text = new GUIText(glm::vec2(0, 0), 0.7f, toolbox->fonts["bold"],
	 "PRESS A TO CONTINUE"
	);
	text->colour = glm::vec4(0.847f, 0.431f, 0.8f, 1.0f);
	leaderboard_items.push_back(gui_manager->add_object(text));
}

void LeaderboardScene::update(float delta_time) {
	gui_manager->update(delta_time);
	int size = 5;	//Y gap
	for (int i = 0; i < leaderboard_items.size(); i+=2) {
		for (int j = 0; j < 2; j++) {	//Items are in groups of 2
			//Checking for overflow in case of odd number of items
			if (i + j >= leaderboard_items.size()) break;
			GUIObject* o = leaderboard_items[i+j];
			float t = max(0, min(time * 5.0f - i/2, 1.0f));
			o->position.x = 25.0f - (25.0f+j*7) * t;
			o->position.y = size - i / 2;
		}
	}
	time += delta_time;

	if (time > 4) {	//After 4 seconds, let the user exit
		if (toolbox->controllers[0]->buttons_pressed[0]) {
			if (toolbox->state.cTrack >= toolbox->tracks.size()-1 || !toolbox->settings.tournament) {
				change_scene(new PodiumScene(
					gui_manager, entity_manager, toolbox),
					toolbox->transitions["swipe"]
				);
			}
			else {
				toolbox->sound_manager->stop_music(true);
				toolbox->state.cTrack++;
				toolbox->state.current_track = 
					toolbox->tracks[toolbox->state.cTrack];
				change_scene(
					new GameScene(
						gui_manager,
						entity_manager,
						toolbox
					),
					 toolbox->transitions["swipe"]
					);
			}
		}
	}

}

void LeaderboardScene::render() {
	gui_manager->render();

	gui_manager->late_render();
}