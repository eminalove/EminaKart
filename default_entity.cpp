#include "default_entity.h"

DefaultEntity::DefaultEntity(glm::vec3 position, Renderable* model) {
	this->model = model;
	this->transform.position = position;
	//Default values
	transform.scale = glm::vec3(1);
}
void DefaultEntity::update(float delta_time) {}

