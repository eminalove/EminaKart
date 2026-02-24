#include "sky_shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

SkyShader::SkyShader(Camera* camera) {
	load_shader("sky");
	this->camera = camera;

	uProj = glGetUniformLocation(program_id, "projection");
	uView = glGetUniformLocation(program_id, "view");
}
void SkyShader::upload_uniforms() {
	glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(camera->projection_matrix));
	glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(camera->untranslated_view_matrix));
}

