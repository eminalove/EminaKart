#pragma once

#include "config.h"
#include "generic_controller.h"


class Controller : public GenericController {
public:
	
	//Which joystick is it
	uint16_t joystick;

	Controller(uint16_t joystick);

	//Poll the controllers buttons and axes
	void update();

};