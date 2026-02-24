#pragma once
#include "config.h"
#include "shader_program.h"
#include "camera.h"
#include "entity.h"

class EntityShader : public ShaderProgram {
public:
	EntityShader(Camera* camera);
	void upload_uniforms() override;
	Entity* current_entity;

	Camera* camera;
protected:
	//Shader variables
	svar uModel;	//Model matrix
	svar uProj;		//Projection matrix
	svar uView;		//View matrix
	svar uCol;		//Colour
};