#include "track.h"

/*Takes in the formatted name, the
 file name, the scale of the model,
 the skybox texutre, the model textures
 as a list and the position that the 
 racers should be spawned from.
*/
Track::Track(std::string name,
	std::string model_name,
	float scale, Texture* sky,
	std::vector<std::string> textures,
	glm::vec3 start_pos,
	SoLoud::Wav* music, float loop_point,
	glm::vec2 map_pos) {

	this->map_pos = map_pos;
	this->name = name;
	this->model_name = model_name;
	this->scale = scale;
	this->track_model = nullptr;
	this->sky = sky;
	this->texture_names = textures;
	this->start_pos = start_pos;
	this->music = music;
	this->loop_point = loop_point;

	this->preview = new Texture("tracks/previews/" + model_name, GL_LINEAR);
}
//Loads in the actual data for the track
void Track::load() {
	loaded = true;
	//Load in all the textures for the MultitextureModel
	std::vector<Texture*> textures;
	for (int i = 0; i < texture_names.size(); i++) {
		textures.push_back(new Texture("tracks/" + 
			model_name + "/" + 
			texture_names[i], GL_LINEAR));
	}
	//Load in the visual model
	track_model = new MultitextureModel(OBJLoader::loadOBJ("tracks/" 
		+ model_name, false),textures);
	//Load in the collision model
	col_model = OBJLoader::loadOBJ("tracks/" 
		+ model_name+"col", true);

	//Load checkpoints
	std::cout << "\033[33mLoading track: \033[36m " + name + "\033[37m\n";
	std::ifstream file("res/tracks/" + model_name + ".track");
	std::string line;
	while (std::getline(file, line)) {
		std::vector<std::string> split = OBJLoader::split(line, ' ');
		BoundingBox box;
		box.position = glm::vec3(std::stof(split[0]), std::stof(split[1]), -std::stof(split[2]))*scale;
		box.rot_y = std::stof(split[3]);
		box.scale = glm::vec3(std::stof(split[4]), std::stof(split[5])*5.0f, std::stof(split[6])) * scale;
		checkpoints.push_back(box);
		//std::cout << box.scale.x << ", " << box.scale.y << ", " << box.scale.z << "\n";
	}
	file.close();
}
//Free the memory
void Track::unload() {
	loaded = false;
	delete track_model;
	track_model = nullptr;
}
//Takes in a ray and returns a triangle, where it hit 
//		  and the interpolated normal of where it hit.
float Track::intersectRay(Ray ray, Triangle& out_tri,
	glm::vec3& position, glm::vec3& norm) {
	float shortest_dist = -1;
	int shortest=0;
	bool hit = false;
	//Go through every triangle
	for (int i = 0; i < col_model->triangles.size(); i++) {
		//Current triangle
		Triangle& tri = col_model->triangles[i];
		//Check for an intersection
		float dist = intersectRayTriangle(ray, tri);
		
		//If the distance is greater than 0 (positive ray direction)
		//and it is the shortest distance so far or the first one,
		//set it as the shortest
		if (dist >= 0 && (shortest_dist <0 || dist< shortest_dist)) {
			shortest_dist = dist;
			hit = true;
			shortest = i;
		}
	}
	//Did the ray hit?
	if (hit) {
		//Set the hit triangle
		out_tri = col_model->triangles[shortest];
		//Get the hit position by adding the direction * the distance
		// to the origin
		glm::vec3 pos = (ray.origin + ray.direction * shortest_dist);
		//Short-hand variable names
		glm::vec3 v0 = out_tri.b - out_tri.a;
		glm::vec3 v1 = out_tri.c - out_tri.a;
		//Remember to descale it
		glm::vec3 v2 = pos/scale - out_tri.a;

		//Barycentric interpolation for the triangle
		float d00 = glm::dot(v0, v0);
		float d01 = glm::dot(v0, v1);
		float d11 = glm::dot(v1, v1);
		float d20 = glm::dot(v2, v0);
		float d21 = glm::dot(v2, v1);

		float denom = d00 * d11 - d01 * d01;
		float v = (d11 * d20 - d01 * d21) / denom;	//Weight for v1
		float w = (d00 * d21 - d01 * d20) / denom;	//Weight for v2
		float u = 1.0f - v - w;						//Weight for v0
		//u,v,w
		position = pos;
		//Interpolated normal
		norm = glm::normalize(out_tri.nA * u + out_tri.nB * v + out_tri.nC * w);
	}
	return shortest_dist;
}