#pragma once

#include "controller.h"
#include "config.h"

class KeyboardController : public GenericController {
public:
	KeyboardController(GLFWwindow* window);
	void update();
private:
	GLFWwindow* window;
	uint16_t keys[MAX_BUTTONS];
};