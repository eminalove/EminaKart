#pragma once

#include "config.h"
#include "shader_program.h"
#include "particle.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

class ParticleShader : public ShaderProgram{
public:
	ParticleShader(Camera* camera) {
		load_shader("particle");
		this->camera = camera;
		current_particle = nullptr;
		//Shader uniform variables
		transformationU = glGetUniformLocation(program_id, "model");
		projU = glGetUniformLocation(program_id, "projection");
		viewU = glGetUniformLocation(program_id, "view");

		aspectU = glGetUniformLocation(program_id, "aspect");
		scaleU = glGetUniformLocation(program_id, "scale");
		blendU = glGetUniformLocation(program_id, "blend");
		numtexturesU = glGetUniformLocation(program_id, "numTextures");
		curtextureU = glGetUniformLocation(program_id, "curTexture");
		transitionU = glGetUniformLocation(program_id, "transition");
		depthU = glGetUniformLocation(program_id, "depth");
		rotationU = glGetUniformLocation(program_id, "rotation");
		variantsU = glGetUniformLocation(program_id, "variants");
		variantU = glGetUniformLocation(program_id, "variant");
	}
	//Values shared with all particles of one system
	void upload_frame() {
		glUniform1f(aspectU,  1.0f/camera->aspect);
		
		glUniformMatrix4fv(projU, 1, GL_FALSE, glm::value_ptr(camera->projection_matrix));
		glUniformMatrix4fv(viewU, 1, GL_FALSE, glm::value_ptr(camera->view_matrix));
		glUniform1i(numtexturesU, num_textures);
		glUniform1i(variantsU, variants);
	}
	//Values set per particle
	void upload_uniforms() override {
		if (current_particle == nullptr) return;

		//Create an identity matrix
		glm::mat4x4 m_mat = glm::identity<glm::mat4x4>();
		//Translate it to the object position
		m_mat = glm::translate(m_mat, current_particle->position);

		//Upload the model matrix to the GPU
		glUniformMatrix4fv(transformationU, 1, GL_FALSE, glm::value_ptr(m_mat));

		glUniform1f(scaleU, current_particle->scale);
		glUniform1f(blendU, current_particle->opacity);
		glUniform1i(curtextureU, cur_texture);
		glUniform1f(transitionU, transition);
		glUniform1f(depthU, current_particle->depth);
		glUniform1f(rotationU, current_particle->rotation*current_particle->time);
		glUniform1i(variantU, current_particle->variant);
	}
	Particle* current_particle;
	int num_textures;
	int cur_texture;
	float transition;
	int variants;
private:
	Camera* camera;
	svar transformationU;
	svar viewU;
	svar projU;
	svar depthU;
	svar variantsU;
	svar variantU;
	svar aspectU;
	svar scaleU;
	svar blendU;
	svar numtexturesU;
	svar curtextureU;
	svar transitionU;
	svar rotationU;
};