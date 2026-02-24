#include "keyboard_controller.h"

KeyboardController::KeyboardController(GLFWwindow* window){
	this->cursor_position = glm::vec2(0, 0);
	this->window = window;
	//Initialise all the variables
	left_stick = glm::vec2(0);
	right_stick = glm::vec2(0);
	triggers = glm::vec2(0);

	//Check if the joystick is already plugged in
	present = true;
	name = "Keyboard";
	//Initialise the buttons
	for (int i = 0; i < MAX_BUTTONS; i++) {
		buttons[i] = 0;
		buttons_pressed[i] = false;
	}
	keys[0] = GLFW_KEY_SPACE;
	keys[1] = GLFW_KEY_LEFT_SHIFT;

}

void KeyboardController::update() {
	//Update all axes
	int axes_count;
	bool left = glfwGetKey(window, GLFW_KEY_LEFT);
	bool right = glfwGetKey(window, GLFW_KEY_RIGHT);
	bool up = glfwGetKey(window, GLFW_KEY_UP);
	bool down = glfwGetKey(window, GLFW_KEY_DOWN);

	//Remove drift by ignoring absolute values under 0.3f
	left_stick.x = left*-1 + right;
	left_stick.y = up * -1 + down;
	
	for (int i = 0; i < MAX_BUTTONS; i++) {
		bool old_state = buttons[i];
		buttons[i] = glfwGetKey(window, keys[i]);
		//Button just pressed
		if (buttons[i] && !old_state) {
			buttons_pressed[i] = true;
		}
		else {
			buttons_pressed[i] = false;
		}
	}
	triggers = glm::vec2(buttons[1] ? 1.0f : 0.0f);
}