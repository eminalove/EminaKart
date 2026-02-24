#pragma once
#include "config.h"
#include "transform.h"

class Camera {
public:
	Camera(glm::vec3 position);

	//Called every frame to update camera variables
	void update_camera();

	glm::mat4x4 projection_matrix;
	glm::mat4x4 view_matrix;
	glm::mat4x4 untranslated_view_matrix;
	Transform transform;
	float fov, aspect;
	void set_orthographic();
	void set_perspective();
private:
	//Create the projection matrix, run every frame
	glm::mat4x4 generate_projection_matrix();

	glm::mat4x4 create_view_matrix();

	//Variables for the projection matrix
	float near, far;
};

