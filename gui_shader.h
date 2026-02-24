#pragma once
#include "config.h"
#include "shader_program.h"

class GUIShader : public ShaderProgram {
public:
	GUIShader();
	void upload_uniforms() override;
	glm::vec2 position = glm::vec2(0);
	glm::vec2 scale = glm::vec2(0);

	//Some textures can have subtextures
	int cur_sprite = 0;
	int num_sprites = 0;
	
	float aspect=ASPECT;
	glm::vec4 colour = glm::vec4(0);
private:
	//Shader variables needed for rendering
	//a GUI object
	svar uPosition;
	svar uScale;
	svar uAspect;
	svar uCurSprite;
	svar uNumSprites;
	svar uColour;
};