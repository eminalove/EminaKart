#pragma once

#include "config.h"
#include "texture.h"

class Font {
public:
	Font(std::string name, int characters) {
		this->texture = new Texture("gui/fonts/" + name, GL_LINEAR);
		this->characters=characters;
	}
	//Number of characters in the font
	int characters;
	//Returns the index of a character, returns space if none found
	int8_t get_char(char c) {
		if (c == ' ') return 0;
		if (c >= 'A' && c <= 'Z') {
			return (c-'A')+1;
		}
		if (c >= '0' && c <= '9') {
			return (c - '0') + 27;
		}
		switch (c) {
		case '.':
			return 37;
		case ',':
			return 38;
		case '!':
			return 39;
		case ':':
			return 40;
		case '?':
			return 41;
		case '+':
			return 42;
		case '(':
			return 43;
		case ')':
			return 44;
		}
		return 0;
	}
	Texture* texture;
};