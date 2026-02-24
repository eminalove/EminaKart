#include "entity_shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

EntityShader::EntityShader(Camera* camera) {
	load_shader("entity");
	this->camera = camera;
	current_entity = nullptr;

	//Getting references to the shader variables
	uModel = glGetUniformLocation(program_id, "model");
	uProj = glGetUniformLocation(program_id, "projection");
	uView = glGetUniformLocation(program_id, "view");
	uCol = glGetUniformLocation(program_id, "colour");
}
void EntityShader::upload_uniforms() {
	if (current_entity == nullptr) return;
	//Create an identity matrix
	glm::mat4x4 m_mat = glm::translate(glm::mat4(1.0f), current_entity->transform.world_position)
		* glm::mat4_cast(current_entity->transform.world_rotation)
		* glm::scale(glm::mat4(1.0f), current_entity->transform.world_scale);
	
	glUniform4f(uCol, current_entity->colour.x, current_entity->colour.y, current_entity->colour.z, current_entity->colour.w);
	//Upload the model matrix to the GPU
	glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(m_mat));
	glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(camera->projection_matrix));
	glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(camera->view_matrix));
}

