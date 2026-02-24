#pragma once

#include "config.h"
#include "scene.h"

class SceneManager {
public:
	//Currently loaded scene
	Scene* loaded_scene;

	SceneManager(Scene* starting_scene, Toolbox* toolbox);

	//Loads the scene and clears old entities
	void load_scene(Scene* scene);
	void update(float delta_time);
	void render();
	void post_render();
private:
	bool changing_scene = false;
	bool detransitioning = false;
	float c_time=0.0f;
	float transition_time=0.0f;
	float transition_time_full = 0.0f;
	Transition* transition = nullptr;
	gvar square_vao;
	bool just_loaded;
};