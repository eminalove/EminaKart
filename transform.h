#pragma once

#include "config.h"

//Represents the transformation of a point in the world
struct Transform {
	//Local coordinates
	glm::vec3 position;
	glm::quat rotation = glm::quat(1,0,0,0);
	glm::vec3 scale = glm::vec3(1.0f);

	//Global coordinates, calculated externally
	glm::vec3 world_position;
	glm::vec3 world_scale;
	glm::quat world_rotation;

	//Matrices for transforming the model on the GPU
	glm::mat4 world_transform;
	glm::mat4 local_transform;

	void update_world_pos(Transform parent) {
		//Calculate the local model matrix
		local_transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::mat4_cast(rotation) *
			glm::scale(glm::mat4(1.0f), scale);

		//Calculate the world matrix
		glm::mat4 trans = parent.world_transform * local_transform;
		world_position = glm::vec3(trans[3]);
		world_scale = glm::vec3(glm::length(trans[0]), glm::length(trans[1]), glm::length(trans[2]));
		glm::mat3 rotationMatrix = glm::mat3(trans);
		rotationMatrix[0] /= world_scale.x;
		rotationMatrix[1] /= world_scale.y;
		rotationMatrix[2] /= world_scale.z;
		world_rotation = glm::quat_cast(rotationMatrix);
	}
	//Rotates the quaternion by euler degrees, rather slow but its convenient
	void rotate(glm::vec3 rotation_degrees) {
		glm::vec3 rot = glm::radians(rotation_degrees);

		glm::quat qx = glm::angleAxis(rot.x, glm::vec3(1, 0, 0));
		glm::quat qy = glm::angleAxis(rot.y, glm::vec3(0, 1, 0));
		glm::quat qz = glm::angleAxis(rot.z, glm::vec3(0, 0, 1));

		glm::quat delta = qz * qy * qx;

		rotation = glm::normalize(delta * rotation);
	}
	//Sets the quaternion by euler degrees
	void set_rotation_degrees(glm::vec3 rotation_degrees) {
		glm::vec3 rot = glm::radians(rotation_degrees);
		glm::quat qx = glm::angleAxis(rot.x, glm::vec3(1, 0, 0));
		glm::quat qy = glm::angleAxis(rot.y, glm::vec3(0, 1, 0));
		glm::quat qz = glm::angleAxis(rot.z, glm::vec3(0, 0, 1));
		glm::quat delta = qz * qy * qx;
		rotation = glm::normalize(delta);

	}
	//Some getters for convenience
	glm::vec3 get_euler_radians() {
		return glm::eulerAngles(rotation);
	}
	glm::vec3 get_euler_degrees() {
		return glm::degrees(glm::eulerAngles(rotation));
	}
	glm::vec3 get_world_euler_degrees() {
		return glm::degrees(glm::eulerAngles(world_rotation));
	}
	glm::vec3 get_world_euler_radians() {
		return glm::eulerAngles(world_rotation);
	}
};