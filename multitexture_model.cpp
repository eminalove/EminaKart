#include "multitexture_model.h"

MultitextureModel::MultitextureModel(Model* model, std::vector<Texture*> textures) {
	this->model = model;
	this->textures = textures;
}
void MultitextureModel::bind() {
	model->bind();
}
void MultitextureModel::render() {
	int start = 0;
	for (int i = 0; i < model->textures.size(); i++) {
		//How many triangles have this texture?
		int texLength = model->textures[i];
		//Bind the texture
		textures[i]->bind(GL_TEXTURE0);
		//Render the triangles used by this texture, they are in order luckily
		model->render_section(start, texLength);
		//Go to the triangle with the next texture!
		start += texLength;
	}
}
void MultitextureModel::clean_up() {
	delete model;
	for (int i = textures.size()-1; i >= 0; i--) {
		delete textures[i];
	}
}