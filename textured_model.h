#pragma once
#include "config.h"
#include "model.h"
#include "texture.h"
#include "renderable.h"

//Simply a texture and a model in one class for convenience
class TexturedModel : public Renderable{
public:
	TexturedModel(Model* model, Texture* texture);
	void bind() override;
	void render() override;
	void clean_up();
	Model* model;
	Texture* texture;
};

