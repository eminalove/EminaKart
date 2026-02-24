#include "gui_shader.h"

GUIShader::GUIShader() {
	load_shader("gui");
	//Uniform locations for all the shader variables
	uPosition = glGetUniformLocation(program_id, "position");
	uAspect = glGetUniformLocation(program_id, "aspect");
	uScale = glGetUniformLocation(program_id, "scale");
	uAspect = glGetUniformLocation(program_id, "aspect");
	uColour = glGetUniformLocation(program_id, "color");
	uCurSprite = glGetUniformLocation(program_id, "curSprite");
	uNumSprites = glGetUniformLocation(program_id, "numSprites");
}

void GUIShader::upload_uniforms() {
	//Just uploading all the GUI object's properties to the GPU
	glUniform2f(uPosition, position.x, position.y);
	glUniform2f(uScale, scale.x, scale.y);
	glUniform1f(uAspect, aspect);
	glUniform1i(uCurSprite, cur_sprite);
	glUniform1i(uNumSprites, num_sprites);
	glUniform4f(uColour, colour.x, colour.y, colour.z, colour.w);


}
