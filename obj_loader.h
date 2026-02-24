#pragma once
#include "config.h"
#include "model.h"

class OBJLoader {
public:
	//Simple structure to store all the vertices while loading a model
	static struct Vertex {
	public:
		glm::vec3 position;
		unsigned int texture_index;
		unsigned int normal_index;
		int duplicate_index;
		unsigned int index;
		//Is the vertex used?
		bool is_set() {
			return texture_index != -1 && normal_index != -1;
		}
		//Are the texture and normal indices the same?
		bool t_n_same(unsigned int texture, unsigned int normal) {
			return texture == texture_index && normal == normal_index;
		}
		Vertex(int index, glm::vec3 position) {
			Vertex::index = index;
			Vertex::position = position;
			texture_index = 0;
			normal_index = 0;
			duplicate_index = -1;
		}
	};
	static std::vector<std::string> split(const std::string& str, char delimiter) {
		std::vector<std::string> result;
		size_t start = 0;
		size_t end = str.find(delimiter);

		while (end != std::string::npos) {
			result.push_back(str.substr(start, end - start));
			start = end + 1;
			end = str.find(delimiter, start);
		}
		result.push_back(str.substr(start, end - start));

		return result;
	}
	//Called if a vertex is already being used
	static void duplicate_vertex(Vertex& previous_vertex, int texture_index, int normal_index, std::vector<Vertex>& vertices, std::vector<int>& indices) {
		//If the texture and normal coordinates are the same, it doesnt matter
		if (previous_vertex.t_n_same(texture_index, normal_index)) {
			indices.push_back(previous_vertex.index);
		}
		else {	//If not we have a problem. We have to create a new vertex
			int new_vertex_index = previous_vertex.duplicate_index;
			if (new_vertex_index != -1) {
				Vertex& new_vertex = vertices[new_vertex_index];
				
				duplicate_vertex(new_vertex, texture_index, normal_index, vertices, indices);
			}
			else {
				Vertex duplicate(vertices.size(), previous_vertex.position);
				duplicate.texture_index = texture_index;
				duplicate.normal_index = normal_index;
				previous_vertex.duplicate_index = duplicate.index;
				vertices.push_back(duplicate);
				indices.push_back(duplicate.index);
			}
		}
	}
	//Extracts all the data out of a singular vertex in an index line
	static Vertex& processVertex(std::vector<std::string>& vertex, std::vector<Vertex>& vertices, std::vector<int>& indices) {
		int index = std::stoi(vertex[0]) - 1;
		Vertex& current_vertex = vertices[index];
		int texture_index = std::stoi(vertex[1]) - 1;
		int normal_index = std::stoi(vertex[2]) - 1;
		if (!current_vertex.is_set()) {
			current_vertex.texture_index = texture_index;
			current_vertex.normal_index = normal_index;
			indices.push_back(index);
		}
		else {
			//Handle vertices that are already being used
			duplicate_vertex(current_vertex, texture_index, normal_index, vertices, indices);
		}
		return current_vertex;
	}
	// Load in a model from an OBJ file
	static Model* loadOBJ(std::string name, bool load_triangles=false) {

		std::cout << "\033[33mLoading model: \033[36m " + name + "\033[37m\n";

		//Creates a file stream for the model file
		std::ifstream file("res/models/" + name + ".obj");
		std::string line;

		//All the lists that wil be filled
		std::vector<Vertex> vertices;
		std::vector<glm::vec2> textures;
		std::vector<glm::vec3> normals;
		std::vector<int> indices;

		std::vector<int> texture_ids;

		//Still reading vertices?
		bool reading_v = true;
		int material_counter = 0;
		//A try catch just in case
		try {
			while (std::getline(file, line)) {
				if (reading_v) {
					if (line.rfind("v ", 0) == 0) {			//Vertex position
						std::vector<std::string> numbers = split(line.substr(2), ' ');
						glm::vec3 v(std::stof(numbers[0]), std::stof(numbers[1]), std::stof(numbers[2]));
						vertices.push_back(Vertex(vertices.size(), v));
					}
					else if (line.rfind("vt ", 0) == 0) {	//Texture coordinate
						std::vector<std::string> numbers = split(line.substr(3), ' ');
						textures.push_back(glm::vec2(std::stof(numbers[0]), std::stof(numbers[1])));
					}
					else if (line.rfind("vn ", 0) == 0) {	//Normal vector
						std::vector<std::string> numbers = split(line.substr(3), ' ');
						normals.push_back(glm::vec3(std::stof(numbers[0]), std::stof(numbers[1]), std::stof(numbers[2])));
					}
					else if (line.rfind("f ", 0) == 0) {	//No longer reading in vertex data
						reading_v = false;
					}
				}
				if (!reading_v) {
					//If no longer reading in indices stop
					if (line.rfind("s ")==0) continue;
					if (!(line.rfind("f ") || line.rfind("usemtl "))) break;
					if (line.rfind("usemtl") != 0) {
						std::vector<std::string> groups = split(line.substr(2), ' ');

						std::vector<std::string> ver1 = split(groups[0], '/');
						std::vector<std::string> ver2 = split(groups[1], '/');
						std::vector<std::string> ver3 = split(groups[2], '/');

						//Push through the vertices for processing
						processVertex(ver1, vertices, indices);
						processVertex(ver2, vertices, indices);
						processVertex(ver3, vertices, indices);

						material_counter++;
					}
					else {
						texture_ids.push_back(material_counter);
						material_counter = 0;
					}
				}
			}
			texture_ids.push_back(material_counter);
		}
		catch (...) {
			std::cerr << "Error reading file!" << std::endl;
			exit(-1);
		}
		for (Vertex& vertex : vertices) {
			//If the vertex isnt used these default to -1,
			//	so to avoid errors I set these to 0 here
			if (!vertex.is_set()) {
				vertex.texture_index = 0;
				vertex.normal_index = 0;
			}
		}
		//The final list of vertices
		std::vector<float> vertices_out;
		//Loop through all the vertices
		for (int i = 0; i < vertices.size(); i++) {
			const Vertex& v = vertices[i];
			glm::vec3 position = v.position;
			glm::vec2 textureCoord = textures[v.texture_index];

			glm::vec3 normalVector = normals[0];
			if (v.normal_index < normals.size()) {
				normalVector = normals[v.normal_index];
			}
			else {
				std::cout << v.normal_index << "Incorrect Normal!" << normals.size() << std::endl;
			}

			//Put all the data into the vertices array

			vertices_out.push_back(position.x);
			vertices_out.push_back(position.y);
			vertices_out.push_back(position.z);

			vertices_out.push_back(normalVector.x);
			vertices_out.push_back(normalVector.y);
			vertices_out.push_back(normalVector.z);

			vertices_out.push_back(textureCoord.x);
			vertices_out.push_back(1 - textureCoord.y);
		}
		//Return the model
		return new Model(vertices_out, indices, texture_ids,load_triangles);
	}
};