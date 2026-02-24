#pragma once

#include "config.h"
#include "multitexture_model.h"
#include "obj_loader.h"
#include "texture.h"
#include "soloud_wav.h"

struct Ray {
	glm::vec3 origin;
	glm::vec3 direction;
};

struct BoundingBox {
    glm::vec3 position;
    float rot_y;
    glm::vec3 scale;
};

class Track {
public:
	Track(std::string name, std::string model_name, float scale,
    Texture* sky, std::vector<std::string> textures,
    glm::vec3 start_pos, SoLoud::Wav* music,
    float loop_point, glm::vec2 map_offset = glm::vec2(0, 0));
	std::string name;
	Texture* sky;
	float scale;
	bool loaded = false;
	void load();
	void unload();
	MultitextureModel* track_model;
	Model* col_model;
    float intersectRay(Ray ray, Triangle& out_tri,
    glm::vec3& pos, glm::vec3& norm);
    glm::vec3 start_pos;
    glm::vec2 map_pos;
    std::vector<BoundingBox> checkpoints;
    SoLoud::Wav* music;
    float loop_point;
    Texture* preview;
private:
	std::string model_name;
	std::vector<std::string> texture_names;
    
    //An implementation of the ray vs triangle intersection
    float intersectRayTriangle(Ray ray, Triangle triangle){
        const float EPSILON = 1e-6f;

        glm::vec3 v0 = triangle.a * scale;
        glm::vec3 v1 = triangle.b * scale;
        glm::vec3 v2 = triangle.c * scale;

        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;

        glm::vec3 h = glm::cross(ray.direction, edge2);
        float a = glm::dot(edge1, h);

        if (fabs(a) < EPSILON)
            return -1;

        float f = 1.0f / a;
        glm::vec3 s = ray.origin - v0;
        float u = f * glm::dot(s, h);

        if (u < 0.0f || u > 1.0f)
            return -1;

        glm::vec3 q = glm::cross(s, edge1);
        float v = f * glm::dot(ray.direction, q);

        if (v < 0.0f || u + v > 1.0f)
            return -1;

        float t = f * glm::dot(edge2, q);

        if (t > EPSILON)
            return t;

        return -1;
    }
};