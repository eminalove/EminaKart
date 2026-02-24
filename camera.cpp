#include "camera.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

Camera::Camera(glm::vec3 position) {
	this->transform.position = position;

	//Defaults for all these variables
	this->transform.set_rotation_degrees(glm::vec3(0, 0, 0));
	this->fov = 90.0f;
	this->aspect = 1920.0f / 1080.0f;
	this->near = 0.08f;
	this->far = 800.0f;
	projection_matrix = generate_projection_matrix();
}
glm::mat4x4 Camera::generate_projection_matrix() {
	//Creates the projection matrix based on a perspective projection
	return glm::perspectiveFov(M_PI * (fov / 180.0f), aspect, 1.0f, near, far);
}
//Creates the view matrix each frame
glm::mat4x4 Camera::create_view_matrix() {
	//Creates the inverted rotational part of the view matrix
	glm::mat4x4 matrix = glm::toMat4(glm::conjugate(transform.rotation));
	//Set this matrix before translating it
	untranslated_view_matrix = matrix;
	//Translate the matrix
	matrix = glm::translate(matrix, -transform.position);
	return matrix;
}
//Refresh projection matrix in case the FOV or aspect ratio has changed
void Camera::update_camera() {
	projection_matrix = generate_projection_matrix();
	view_matrix = create_view_matrix();
}

void Camera::set_orthographic() {
	projection_matrix = glm::ortho(-WIDTH/2, WIDTH/2, -HEIGHT/2, HEIGHT/2,-100.0f,100.0f);
}
void Camera::set_perspective() {
	projection_matrix = generate_projection_matrix();
}
