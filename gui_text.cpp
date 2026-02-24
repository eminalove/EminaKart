#include "gui_text.h"

GUIText::GUIText(glm::vec2 position, float size, Font* font, std::string text) {
	this->position = position;
	this->size = size;
	this->font = font;
	this->text = text;
	//First refresh to create the initial list of indices
	refresh();
}

//Update the letters list with the new string
void GUIText::refresh() {
	letters.clear();
	for (int i = 0; i < text.length(); i++) {
		letters.push_back(font->get_char(text[i]));
	}
}

void GUIText::render(gvar square_vao, GUIShader* shader) {
	//All the per-text shader variables
	shader->position.y = position.y;
	shader->scale = glm::vec2(size*0.6f,size)*0.5f;
	shader->num_sprites = font->characters;
	//Bind the texture
	font->texture->bind(GL_TEXTURE0);
	//Loop through and render all the characters
	for (int i = 0; i < letters.size(); i++) {
		//All the per-character shader variables

		//Characters x position should increase each iteration
		shader->position.x = position.x + i * size*0.6f;
		shader->colour = colour;
		shader->cur_sprite = letters[i];
		//Upload the variables
		shader->upload_uniforms();
		//Render the square
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
}