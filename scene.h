#pragma once

#include "config.h"
#include "entity_manager.h"
#include "gui_manager.h"
#include "toolbox.h"


class Scene {
public:
	//All the classes needed by scenes that exist throughout scene changes
	Scene(GUIManager* gui, EntityManager* entities, Toolbox* toolbox) {
		this->gui_manager = gui;
		this->entity_manager = entities;
		this->toolbox = toolbox;
	}
	//Called before the first update
	virtual void start() = 0;
	
	//Ran every frame
	virtual void update(float delta_time) = 0;

	//Draws the scene
	virtual void render() = 0;

	//Make the scene manager change scenes after this frame
	void change_scene(Scene* next_scene, Transition* transition) {
		this->next_scene = next_scene;
		this->transition = transition;
	}

	GUIManager* gui_manager;
	EntityManager* entity_manager;
	Toolbox* toolbox;

	//Scene to be changed to. Shouldn't be modified directly
	Scene* next_scene = nullptr;
	Transition* transition = nullptr;
};