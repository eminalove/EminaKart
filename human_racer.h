#pragma once
#include "racer.h"

//A racer controlled by a person
class HumanRacer : public Racer {
public:
	HumanRacer(glm::vec3 position, Toolbox* toolbox, GenericController* controller, Camera* camera, int place);
	void update(float delta_time);
private:
	GenericController* controller;
};