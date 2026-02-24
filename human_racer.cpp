#include "human_racer.h"

HumanRacer::HumanRacer(glm::vec3 position, Toolbox* toolbox, GenericController* controller, Camera* camera, int place) :
	Racer(position, toolbox, camera, place,true) {
	this->controller = controller;
	
}
void HumanRacer::update(float delta_time) {
	__super::update(delta_time);

	if (frozen) return;
	controller->update();
	//Should accelerate if the player presses A
	should_accelerate = controller->buttons[0];
	//Drift on trigger pressed enough
	should_drift = controller->triggers.y > 0.3f;
	left_stick = controller->left_stick.x;
}