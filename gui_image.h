#pragma once

#include "config.h"
#include "gui_object.h"

class GUIImage : public GUIObject {
public:
	GUIImage(Texture* texture, glm::vec2 position, glm::vec2 scale) {
		//Set the variables and some defaults
		this->texture = texture;
		this->position = position;
		this->scale = scale;
		current_sprite = 0;
		num_sprites = 1;
	}
	~GUIImage() {

	}
	//Some test code to transform the object in all its axes
	void update(float delta_time) override {}
	//Just in case the memory isnt freed externally
	void cleanup() {
		delete texture;
	}
};