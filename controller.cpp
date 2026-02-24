#include "controller.h"

Controller::Controller(uint16_t joystick) {
	this->joystick = joystick;
	this->cursor_position = glm::vec2(0, 0);
	//Initialise all the variables
	left_stick = glm::vec2(0);
	right_stick = glm::vec2(0);
	triggers = glm::vec2(0);

	//Check if the joystick is already plugged in
	present = glfwJoystickPresent(joystick);
	if (present) {
		//If so, get the joysticks name
		name = glfwGetJoystickName(joystick);
	}
	//Initialise the buttons
	for (int i = 0; i < MAX_BUTTONS; i++) {
		buttons[i] = 0;
		buttons_pressed[i] = false;
	}
	
}

void Controller::update() {
	int was_present = present;
	present = glfwJoystickPresent(joystick);
	if (was_present && !present) {

		std::cout << "\033[31mJoystick " << (joystick+1) <<" Disconnected: \033[37m" << name << "\n";
	}
	if (present) {
		//Just connected this frame, update the name
		if (!was_present) {
			name = glfwGetJoystickName(joystick);
			std::cout << "\033[32mJoystick " << (joystick + 1) << " Connected: \033[37m" << name << "\n";
		}

		//Update all axes
		int axes_count;
		const float* axes = glfwGetJoystickAxes(joystick, &axes_count);
		//Remove drift by ignoring absolute values under 0.3f
		left_stick.x = abs(axes[0])>0.3f ? axes[0] : 0.0f;
		left_stick.y = abs(axes[1]) > 0.3f ? axes[1] : 0.0f;

		right_stick.x = abs(axes[2]) > 0.3f ? axes[2] : 0.0f;
		right_stick.y = abs(axes[3]) > 0.3f ? axes[3] : 0.0f;

		triggers.x = abs(axes[4]) > 0.3f ? axes[4] : 0.0f;
		triggers.y = abs(axes[5]) > 0.3f ? axes[5] : 0.0f;

		//Update all buttons
		int count;
		const unsigned char* new_buttons = glfwGetJoystickButtons(joystick, &count);
		int max = count > MAX_BUTTONS ? MAX_BUTTONS : count;
		for (int i = 0; i < max; i++) {
			bool old_state = buttons[i];
			buttons[i] = new_buttons[i]==GLFW_PRESS;
			//Button just pressed
			if (buttons[i] && !old_state) {
				buttons_pressed[i] = true;
			}
			else {
				buttons_pressed[i] = false;
			}
		}
	}
	else {
		//Reset everything if the controller is disconnected
		left_stick.x = 0;
		left_stick.y = 0;
		right_stick.x = 0;
		right_stick.y = 0;
		triggers.x = 0;
		triggers.y = 0;
		for (int i = 0; i < MAX_BUTTONS; i++) {
			buttons[i] = 0;
			buttons_pressed[i] = false;
		}
	}
}