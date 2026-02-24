#include "test_shader.h"

TestShader::TestShader() {
	load_shader("test");
	t = 0;
	//Get the variable location
	uTime = glGetUniformLocation(program_id, "time");
}

void TestShader::upload_uniforms() {
	//Upload the variable and increase it
	glUniform1f(uTime, t);
	t += 0.001f;
}

