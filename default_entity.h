#pragma once
#include "entity.h"

//A simple entity that just displays its model where it should be;
class DefaultEntity : public Entity {
public:
	DefaultEntity(glm::vec3 position, Renderable* model);
	void update(float delta_time) override;
	void render() override {}
};