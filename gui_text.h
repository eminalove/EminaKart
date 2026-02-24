#pragma once

#include "font.h"
#include "config.h"
#include "gui_object.h"

class GUIText : public GUIObject {
public:
	GUIText(glm::vec2 position, float size, Font* font, std::string text);
	void render(gvar square_vao, GUIShader* shader) override;
	//Remake the letters list
	void refresh();
	float size;
	std::string text;
private:
	std::vector<int8_t> letters;
	Font* font;
};