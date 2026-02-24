#pragma once

#include "entity.h"
#include "particle.h"
#include "particle_template.h"
#include "particle_shader.h"

class ParticleSystem : public Entity {
public:
	ParticleSystem(glm::vec3 position, ParticleTemplate* particle_template, ParticleShader* shader, gvar square_vao, Camera* cam);

	void update(float delta_time) override;
	void render() override;
	bool emitting = true;

private:
	//Variables for a particle system
	ParticleTemplate* particle_template;
	ParticleShader* shader;
	gvar square_vao;

	Camera* cam;
	//Emit a single particle or burst
	void emit();
	void sort();
	//All particles currently alive
	std::vector<Particle> particles;

	float p_time;
};