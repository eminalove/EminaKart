#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(std::string name, int type) {
	std::cout << "\033[33mLoading texture: \033[36m" << name << "\033[37m\n";

	//Load in the raw data from the file
	unsigned char* data = stbi_load(("res/textures/" + name + ".png").c_str(), &width, &height, &nChannels, 0);

	//Get a pointer to a new texture on the GPU
	glGenTextures(1, &texture_id);

	glBindTexture(GL_TEXTURE_2D, texture_id);
	//Upload the texture data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	//What should happen if the texture is stretched?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, type);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, type);

	//Repeat the texture for coordinates outside the normal UV range of 0-1.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	//Free the CPU data, its all on the GPU
	stbi_image_free(data);
}
//Free GPU memory
Texture::~Texture() {
	glDeleteTextures(1, &texture_id);
}

//Bind the texture
void Texture::bind(int unit) {
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_2D, texture_id);
}