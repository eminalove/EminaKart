#pragma once
#include "config.h"
#include "renderable.h"

struct Triangle {
	glm::vec3 a;
	glm::vec3 b;
	glm::vec3 c;

	glm::vec3 nA, nB, nC;
	glm::vec3 norm;
	int material;
};

class Model : public Renderable{
public:
	Model(std::vector<float> vertices, std::vector<int> indices, std::vector<int> textures, bool load_triangles=false);
	~Model();
	//Bind to the GPU
	void bind() override;
	void unbind();
	//Renders the model
	void render() override;
	void render_section(int start, int length);
	gvar vertex_count, index_count;
	//Doesnt necessarily exist, holds all triangles
	std::vector<Triangle> triangles;
	std::vector<int> textures;
private:
	gvar vao_id;
	gvar vbo_id;
	gvar index_id;
};

