#pragma once

#include "config.h"
#include "character_type.h"
#include "textured_model.h"

struct Character {
public:
	std::string name;

	//Should be exactly 8 characters for the selection screen
	std::string short_name;

	TexturedModel* model;

	//The enum to address it
	CharacterType::CharacterType type;

	Character(std::string name, std::string short_name, CharacterType::CharacterType type, TexturedModel* model) {
		this->name = name;
		this->short_name = short_name;
		this->type = type;
		this->model = model;
	}
};