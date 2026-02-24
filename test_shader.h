#pragma once

#include "shader_program.h"

class TestShader : public ShaderProgram {
public:
	TestShader();
	void upload_uniforms() override;

private:
	float t;
	unsigned int uTime;
};