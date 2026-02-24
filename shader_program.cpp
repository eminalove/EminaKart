#include "shader_program.h"

void ShaderProgram::load_shader(std::string name) {
	load_shader(name, name);
}
void ShaderProgram::load_shader(std::string vert_name, std::string frag_name) {
	std::cout << "\033[33mLoading shader: \033[36m" << vert_name << " \033[33m/\033[36m " << frag_name << "\033[37m\n";

	//Create the vertex and fragment stages
	unsigned int vertex = create_module(vert_name, GL_VERTEX_SHADER);
	unsigned int fragment = create_module(frag_name, GL_FRAGMENT_SHADER);

	//Create and link the shader program
	program_id = glCreateProgram();
	glAttachShader(program_id, vertex);
	glAttachShader(program_id, fragment);
	glLinkProgram(program_id);

	//Check for errors
	int success;
	glGetProgramiv(program_id, GL_LINK_STATUS, &success);
	if (!success) {
		char error[1024];
		glGetProgramInfoLog(program_id, 1024, NULL, error);
		std::cout << "\033[31mError linking " << vert_name << " / " << frag_name << "\n" << error << "\033[37m\n";
	}

	//Clean up
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}
//Clean up
ShaderProgram::~ShaderProgram() {
	glDeleteProgram(program_id);
}
unsigned int ShaderProgram::create_module(std::string name, unsigned int type) {
	//Create the file object
	std::ifstream file;
	//Objects to hold all lines and the current one
	std::stringstream lines;
	std::string line;

	//Shaders are stored in "res/shaders"
	file.open("res/shaders/" + name + (type==GL_VERTEX_SHADER ? ".vert" : ".frag"));
	//Sum all the lines
	while (std::getline(file, line)) {
		lines << line << "\n";
	}
	//Print out the shader source
	std::string source = lines.str();
	const char* src = source.c_str();

	//Clean up
	lines.clear();
	file.close();

	//Create the shader and upload the data to it, then compile
	unsigned int shader_module = glCreateShader(type);
	glShaderSource(shader_module, 1, &src, NULL);
	glCompileShader(shader_module);

	//Check for errors
	int success;
	glGetShaderiv(shader_module, GL_COMPILE_STATUS, &success);
	if (!success) {
		char error[1024];
		glGetShaderInfoLog(shader_module, 1024, NULL, error);
		std::cout << "\033[31mError compiling " << 
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< " shader: " << name << "\n" << error << "\033[37m\n";
	}
	return shader_module;
}

void ShaderProgram::use() {
	glUseProgram(program_id);
}