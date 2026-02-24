#include "entity_manager.h"

EntityManager::EntityManager(Camera* cam) {
	shader = new EntityShader(cam);
}
EntityManager::~EntityManager() {
	delete shader;
}

Entity* EntityManager::add_entity(Entity* entity) {
	//Add an entity and sets the positions
	entities.push_back(entity);
	entity->transform.world_position = entity->transform.position;
	entity->transform.world_rotation = entity->transform.rotation;
	entity->transform.world_scale = entity->transform.scale;
	return entity;
}

//Find the entity and remove it
void EntityManager::remove_entity(Entity* entity) {
	for (int i = 0; i < entities.size(); i++) {
		//Are the pointers the same?
		if (entities[i] == entity) {
			//Remove from the entities list and delete the object
			entities.erase(entities.begin()+i);
			delete entity;
			return;
		}
	}
}
void EntityManager::clear() {
	//Simply clears the whole entity list
	for (int i = 0; i < entities.size(); i++) {
		delete entities[i];
	}
	entities.clear();
}
//Loop through and update all the entities
void EntityManager::update(float delta_time) {
	for (int i = 0; i < entities.size(); i++) {
		if (update_entity(entities[i], delta_time)) { //Returns true if it should be removed
			//Swap with the back one and delete the back entity
			entities[i] = entities.back();
			entities.pop_back();
		}
	}
}
bool EntityManager::update_entity(Entity* entity, float delta_time) {
	entity->update(delta_time);
	if (entity->parent != nullptr) {	//Has a parent? update world position
		entity->transform.update_world_pos(entity->parent->transform);
	}
	else {	//Doesn't have a parent? World position is local position
		entity->transform.world_position = entity->transform.position;
		entity->transform.world_rotation = entity->transform.rotation;
		entity->transform.world_scale = entity->transform.scale;
	}
	//Have to apply the world transform
	entity->transform.world_transform =		glm::translate(glm::mat4(1.0f), entity->transform.world_position) *
											glm::mat4_cast(entity->transform.world_rotation) *
											glm::scale(glm::mat4(1.0f), entity->transform.world_scale);
	for (int i = 0; i < entity->children.size(); i++) {		//Update all the entities children recursively
		if (update_entity(entity->children[i], delta_time)) {
			entity->children[i] = entity->children.back();
			entity->children.pop_back();
		}
	}
	if (entity->marked_for_removal) {	//If the entity should be removed this frame, delete it and return true
		delete entity;
		return true;
	}
	return false;
}
// Just like the other update_entity for just one entity
// Can be used outside of the entity manager if you want
// to update an entity that isn't held in the entity manager.
void EntityManager::update_single_entity(Entity* entity, float delta_time) {	
	entity->update(delta_time);
	if (entity->parent != nullptr) {
		entity->transform.update_world_pos(entity->parent->transform);
	}
	entity->transform.world_transform = glm::translate(glm::mat4(1.0f), entity->transform.world_position) *
		glm::mat4_cast(entity->transform.world_rotation) *
		glm::scale(glm::mat4(1.0f), entity->transform.world_scale);
	for (int i = 0; i < entity->children.size(); i++) {
		if (update_entity(entity->children[i], delta_time)) {
			entity->children[i] = entity->children.back();
			entity->children.pop_back();
		}
	}
	entity->transform.world_position = entity->transform.position;
	entity->transform.world_rotation = entity->transform.rotation;
	entity->transform.world_scale = entity->transform.scale;
	entity->update(delta_time);
}
//Loop through and render all the entities
void EntityManager::render() {
	shader->use();
	for (int i = 0; i < entities.size(); i++) {
		render_entity(entities[i]);

	}
}
void EntityManager::render_entity(Entity* entity) {
	//Uses the default rendering method
	if (entity->renderable) {
		entity->model->bind();
		shader->current_entity = entity;
		shader->upload_uniforms();
		entity->model->render();
	}
	else {	//Has its own rendering method
		entity->render();
		shader->use();
	}
	//Render all its children recursively
	for (int i = 0; i < entity->children.size(); i++) {
		render_entity(entity->children[i]);
	}
}

//Used to render a single entity, can be called from outside the class.
void EntityManager::render_single_entity(Entity* entity) {
	shader->use();
	if (entity->renderable) {
		entity->model->bind();
		shader->current_entity = entity;
		shader->upload_uniforms();
		entity->model->render();
	}
	else {
		entity->render();
		shader->use();
	}
}