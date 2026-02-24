#pragma once
#include "config.h"
#include "scene.h"
#include "entity.h"
#include "gui_button.h"

class MenuScene : public Scene{
public:
	MenuScene(GUIManager* gui, EntityManager* entities, Toolbox* toolbox, bool play_music=false);
	~MenuScene();

	void update(float delta_time) override;
	void render() override;
	void start() override;
private:
	//Spinning logo
	Entity* logo;

	//Buttons for different options
	GUIButton* start_button;
	GUIButton* two_button;
	GUIButton* three_button;
	GUIButton* four_button;
	GUIButton* quit_button;

	GUIButton* options_button;
	bool play_music=false;
};