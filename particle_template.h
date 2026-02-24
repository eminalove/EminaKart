#pragma once

#include "config.h"
#include "texture.h"

struct ParticleTemplate {
public:
	int max_particles;			//How many particles can exist at once
	glm::vec3 initial_velocity;	//What direction should they start at
	bool random_rotation;		//Should the particle start with a random rotation?
	float rotate_over_time;		//How much should the particle rotate over time?
	float spread;				//How much variation in direction should there be
	float frequency;			//How often should they be spawned
	int batch_size;				//How many should be spawned at once
	float time_alive;			//How long should the particles exist
	bool one_shot;				//Should the particles be spawned once?
	float scale_over_time;		//How should the particles scale change over time?
	bool fade;					//Should the particles fade over time?
	float start_size;			//What scale should they start at?
	float gravity;				//How much should they be affected by gravity?
	Texture* texture;			//The particles texture
	int num_textures;			//How many sub sprites in the texture?
	int variants;				//How many variants in the texture?
};

