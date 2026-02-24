#pragma once
#include "config.h"
#include "renderable.h"
#include "transform.h"

class Entity {
public:
	//Contains position, rotation etc...
	Transform transform;
	
	Entity* parent = nullptr;
	std::vector<Entity*> children;
	
	//Uses default rendering pipeline?
	bool renderable = true;
	//Should be deleted next frame?
	bool marked_for_removal = false;

	virtual void update(float delta_time) = 0;
	virtual void render() = 0;
	
	Renderable* model;
	
	//Multiplied by texture colour
	glm::vec4 colour = glm::vec4(1.0f,1.0f,1.0f,1.0f);
	void destroy() {
		marked_for_removal = true;
	}
	Entity* add_child(Entity* child) {
		this->children.push_back(child);
		child->parent = this;
		return child;
	}


};

