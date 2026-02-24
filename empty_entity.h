#pragma once

#include "config.h"
#include "entity.h"

//Some entities needn't have a model, sometimes they are only needed for their transform
class EmptyEntity : public Entity {
public:
	EmptyEntity(glm::vec3 position) {
		this->transform.position = position;
		this->renderable = false;
	}
	void render() override {}
	void update(float delta_time) override {}
};