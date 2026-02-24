#pragma once
#include "config.h"
#include "scene.h"
#include "entity.h"
#include "gui_button.h"

class OptionsScene : public Scene {
public:
	OptionsScene(GUIManager* gui, EntityManager* entities, Toolbox* toolbox);
	~OptionsScene();

	void update(float delta_time) override;
	void render() override;
	void start() override;
private:
	//Spinning cog
	Entity* logo;
	GUIButton* back_button;

	//Setting components
	GUIImage* master_knob;
	GUIImage* master_slider;
	GUIText* master_text;

	GUIImage* music_knob;
	GUIImage* music_slider;
	GUIText* music_text;

	GUIButton* cpu_button;
	GUIButton* tournament_button;

	void refresh();
};