#pragma once
#include "config.h"
#include "model.h"
#include "texture.h"
#include "renderable.h"

class MultitextureModel : public Renderable {
public:
	MultitextureModel(Model* model, std::vector<Texture*> textures);
	void bind() override;
	void render() override;
	void clean_up();
	Model* model;
	//All the textures in the model, rendered individually
	std::vector<Texture*> textures;
};

