#pragma once

#include "config.h"

class ShaderProgram {
public:
	void load_shader(std::string name);
	void load_shader(std::string vert_name, std::string frag_name);
	~ShaderProgram();
	void use();
	virtual void upload_uniforms() = 0;
protected:
	unsigned int program_id;
private:
	// Creates a stage in the rendering pipeline
	unsigned int create_module(std::string name, unsigned int type);
};