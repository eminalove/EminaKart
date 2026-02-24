#pragma once

#include "config.h"

struct Particle {
public:
	//Variables that vary per particle

	glm::vec3 position;
	glm::vec3 velocity;

	float opacity;
	float scale;
	float time;
	float rotation;

	int variant;

	float depth;
};

