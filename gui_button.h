#pragma once

#include "config.h"
#include "gui_object.h"
#include "gui_text.h"

class GUIButton : public GUIObject {
public:
	GUIButton(Texture* texture, glm::vec2 position, glm::vec2 scale,
	glm::vec4 col,glm::vec4 hover_col,
	std::string text, Font* font,
	float font_size,glm::vec4 text_colour) {
		has_text = !text.empty();
		//Set the variables and some defaults
		this->texture = texture;
		this->position = position;
		this->scale = scale;

		this->normal_col = col;
		this->hover_col = hover_col;

		current_sprite = 0;
		num_sprites = 1;
		if (has_text) {
			this->text = new GUIText(position, font_size, font, text);
			this->text->colour = text_colour;
		}
		
	}
	
	~GUIButton() {
	}
	void render(gvar square, GUIShader* shader) override {
		GUIObject::render(square, shader);	//Render the image
		if (!has_text) return;	//If there is text, render it
		text->position.x = position.x - ((text->text.size()+1) * (text->size*0.6f))/2.0;
		text->render(square, shader);
	}
	void update(float delta_time) override {	//Update hover based on colour
		colour = hovering>0 ? hover_col : normal_col;
	}
	//Just in case the memory isnt freed externally
	void cleanup() {
		delete texture;
	}
	void set_text(std::string _text) {	//Sets the text and refreshes
		this->text->text = _text;
		this->text->refresh();
	}

private:
	bool has_text = false;
	glm::vec4 normal_col, hover_col;
	GUIText* text = nullptr;
};