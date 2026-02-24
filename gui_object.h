#pragma once

#include "config.h"
#include "texture.h"
#include "gui_shader.h"

class GUIObject {
public:
	//Attributes of each GUI object
	glm::vec2 position = glm::vec2(0);
	glm::vec2 scale = glm::vec2(1);
	glm::vec4 colour = glm::vec4(1);
	int current_sprite = 0;
	int num_sprites = 1;
	Texture* texture;

	uint8_t hovering = 0;

	//Called every frame to update the object
	virtual void update(float delta_time) {}

	//Called every frame to draw the object to the screen
	virtual void render(gvar square_vao, GUIShader* shader) {
		
		//Set all the shader variables
		shader->position = position;
		shader->scale = scale;
		shader->colour = colour;
		shader->cur_sprite = current_sprite;
		shader->num_sprites = num_sprites;

		//Upload the variables
		shader->upload_uniforms();
		//Bind the texture
		texture->bind(GL_TEXTURE0);
		//Render the square
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
};