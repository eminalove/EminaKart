#pragma once
#include "config.h"
#include "shader_program.h"
#include "camera.h"
#include "entity.h"

//Draws the sky
class SkyShader : public ShaderProgram {
public:
	SkyShader(Camera* camera);
	void upload_uniforms() override;

	Camera* camera;
protected:
	svar uProj;
	svar uView;
};