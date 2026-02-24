#include "particle_system.h"
#include <random>

ParticleSystem::ParticleSystem(glm::vec3 position, ParticleTemplate* particle_template, ParticleShader* shader, gvar square, Camera* cam) {
	this->particle_template = particle_template;
	this->transform.position = position;
	this->shader = shader;
	this->cam = cam;
	this->square_vao = square;

	this->renderable = false;
	p_time = 0.0f;
	particles.reserve(particle_template->max_particles);
	if (particle_template->one_shot) {
		for (int i = 0; i < particle_template->max_particles; i++) {
			emit();
		}
	}
}

void ParticleSystem::update(float delta_time) {
	//If the particle system is in one shot mode, destroy the object if it has expired.
	if (particle_template->one_shot){
		if (particles.size() == 0) {
			destroy();
			return;
		}
	}
	
	p_time += delta_time;
	//Emit a particle if the time has come
	if (emitting && p_time > particle_template->frequency) {
		p_time = 0;
		for (int i = 0; i < particle_template->batch_size; i++) emit();
	}

	//Update all the particles
	for (int i = 0; i < particles.size(); i++) {
		Particle& p = particles[i];
		p.time += delta_time;

		//Do a swap and pop removal if a particle has expired.
		if (p.time > particle_template->time_alive) {
			particles[i] = particles.back();
			particles.pop_back();
			//Have to process this index again
			i--;
			continue;
		}
		//Change all the variables over time
		p.position += p.velocity * delta_time;
		p.velocity.y += particle_template->gravity * delta_time;
		if (particle_template->fade) p.opacity = 
			(p.time / particle_template->time_alive);
		p.scale = particle_template->start_size + 
			(p.time / particle_template->time_alive)
			* particle_template->scale_over_time;

		//Recalculate the depth for sorting
		glm::vec3 toParticle = p.position - cam->transform.position;
		p.depth = glm::dot(toParticle, toParticle);
	}
	//Sort all the particles by depth
	sort();
}
void ParticleSystem::sort() {
	//Sort all the particles by depth
	std::sort(particles.begin(), particles.end(),
		[](const Particle& a, const Particle& b) {
			return a.depth > b.depth;
		});
}
void ParticleSystem::render() {
	//Bind the shader and enable all the necessary OpenGL options
	shader->use();
	glBindVertexArray(square_vao);
	glEnableVertexAttribArray(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);
	//Upload per-system variables
	shader->variants = particle_template->variants;
	particle_template->texture->bind(GL_TEXTURE0);
	shader->num_textures = particle_template->num_textures;
	shader->upload_frame();
	for (int i = 0; i < particles.size(); i++) {
		Particle& p = particles[i];
		//Upload per-particle variables
		shader->current_particle = &p;
		shader->cur_texture = (int)(p.time / particle_template->time_alive 
			* particle_template->num_textures);
		float gap = particle_template->time_alive / particle_template->num_textures;
		float t = p.time - shader->cur_texture * gap;
		shader->transition = t / gap;
		shader->upload_uniforms();

		//Render the square
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	//Clean up
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

void ParticleSystem::emit() {
	//Too many particles!!
	if (particles.size() >= particle_template->max_particles) return;

	//Create and set up a particle
	Particle p;
	p.position = transform.world_position;
	//A random value added on based on the spread variable
	glm::vec3 spread(((rand() % 1000) / 1000.0f-0.5f) * particle_template->spread,
		((rand() % 1000) / 1000.0f - 0.5f) * particle_template->spread,
		((rand() % 1000) / 1000.0f - 0.5f) * particle_template->spread);

	p.velocity = particle_template->initial_velocity + spread;
	p.scale = particle_template->start_size;

	p.opacity = 0.0f;
	p.time = 0.0f;
	p.variant = rand() % particle_template->variants;
	//If the rotation should be randomized, set that up
	if (particle_template->random_rotation)
		p.rotation = ((rand() % 1000) / 500.0f-1.0f)*particle_template->rotate_over_time;
	//Add the particle to the list
	particles.push_back(p);
}

