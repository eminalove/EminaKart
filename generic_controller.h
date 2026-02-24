#pragma once

#include "config.h"
#define MAX_BUTTONS 10

class GenericController {
public:
	virtual void update() = 0;
	//The different axes
	glm::vec2 left_stick;
	glm::vec2 right_stick;

	glm::vec2 cursor_position;
	float cursor_opacity;

	bool hovering = false;

	//X=Left Y=Right
	glm::vec2 triggers;

	//The current state of all buttons
	bool buttons[MAX_BUTTONS];
	//All buttons that have just been pressed
	bool buttons_pressed[MAX_BUTTONS];

	//Is the controller plugged in?
	int present;

	//Name of the controller
	std::string name;
};