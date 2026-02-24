#pragma once

#include "shader_program.h"

class TransitionShader : public ShaderProgram {
public:
	TransitionShader(std::string frag_name) {
		//Vertex shader always the same, fragment varies
		load_shader("transition",frag_name);
		//Get the uniform locations for the variables
		blendU = glGetUniformLocation(program_id, "blend");
		blendfullU = glGetUniformLocation(program_id, "blendFull");
		timeU = glGetUniformLocation(program_id, "time");
	}
	void upload_uniforms() override {
		//Upload time and blend
		glUniform1f(timeU, time);
		glUniform1f(blendU, blend);
		glUniform1f(blendfullU, blend_full);
	}
	//Time since program started
	float time=0;
	//Goes from 0->1->0 over course of transition
	float blend=0;
	float blend_full = 0;
private:
	//Shader variable pointers
	svar blendU;
	svar blendfullU;
	svar timeU;
};