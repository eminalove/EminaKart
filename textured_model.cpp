#include "textured_model.h"

//Takes in a model and a texture
TexturedModel::TexturedModel(Model* model, Texture* texture) {
	this->model = model;
	this->texture = texture;
}
void TexturedModel::bind() {
	texture->bind(GL_TEXTURE0);
	model->bind();
}
void TexturedModel::render() {
	model->render();
}
void TexturedModel::clean_up() {
	delete model;
	delete texture;
}