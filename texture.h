#pragma once
#include "config.h"
class Texture {
public:
	int width, height, nChannels;
	Texture(std::string name, int type);
	~Texture();
	void bind(int texture_unit);
private:
	//A pointer to the texture in GPU memory
	svar texture_id;
};