#pragma once

#include "config.h"
#include "shader_program.h"
#include "transition_shader.h"

//Represents a scene transition, just data of a shader and a speed
class Transition {
public:
	Transition(std::string name, float speed);
	float speed;
	TransitionShader* shader;
};