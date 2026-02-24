#include "transition.h"

//Simply loads the shader and sets the speed
Transition::Transition(std::string name, float speed) {
	this->speed = speed;
	this->shader = new TransitionShader(name);
}