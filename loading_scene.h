#pragma once

#include "config.h"
#include "scene.h"
#include "gui_image.h"
#include "gui_text.h"

class LoadingScene  : public Scene{
public:
	LoadingScene(GUIManager* gui, EntityManager* entities, Toolbox* toolbox);
	~LoadingScene();

	void update(float delta_time) override;
	void render() override;
	void start() override;
private:
	//Grows in X with the progress
	GUIImage* loading_bar;
	//States the current stage
	GUIText* loading_status;
	int update_iteration = -1;
	bool finished_updating = false;
	//Variables for interpolation
	float target_scale = 0.0f;
	float current_scale = 0.0f;
	float update_time = 0.0f;
	bool should_load = true;
};