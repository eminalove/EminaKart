#pragma once

#include "config.h"
#include "scene.h"
#include "entity.h"
#include "gui_button.h"

class CharacterScene : public Scene {
public:
	CharacterScene(GUIManager* gui, EntityManager* entities, Toolbox* toolbox);
	~CharacterScene();

	void update(float delta_time) override;
	void render() override;
	void start() override;
private:
	
	GUIButton* start_button;
	GUIButton* back_button;
	//Grid of character buttons that can be selected
	std::vector<GUIButton*> character_buttons;
	//Selected characters for players 1 to 4
	std::vector<GUIButton*> player_texts;
};