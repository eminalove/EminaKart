#pragma once

#include "config.h"
#include "entity.h"
#include "entity_shader.h"

class EntityManager {
public:
	EntityManager(Camera* cam);
	~EntityManager();

	//Adds an entity to the list
	Entity* add_entity(Entity* entity);
	//Removes an entity and destroys it
	void remove_entity(Entity* entity);
	//Render an entity externally
	void render_single_entity(Entity* entity);
	void clear();
	void update(float delta_time);
	void render();
	//Update an entity externally
	void update_single_entity(Entity* entity, float delta_time);
private:
	//All the entities in a list
	std::vector<Entity*> entities;
	//Default entity shader
	EntityShader* shader;
	void render_entity(Entity* entity);
	bool update_entity(Entity* entity, float delta_time);
};

