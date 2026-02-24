#include "scene_manager.h"

SceneManager::SceneManager(Scene* starting_scene, Toolbox* toolbox) {
	loaded_scene = starting_scene;
	loaded_scene->start();
	square_vao = toolbox->square_vao;
	just_loaded = false;
}

//Deletes current scene and loads and initialises the new one
void SceneManager::load_scene(Scene* scene) {
	delete loaded_scene;
	scene->entity_manager->clear();
	scene->gui_manager->clear();
	loaded_scene = scene;
	loaded_scene->toolbox->camera->transform.position = glm::vec3(0, 0, 0);
	loaded_scene->start();
}

void SceneManager::update(float delta_time) {
	c_time += delta_time;
	if (!changing_scene) {
		//Update the current scene
		loaded_scene->update(delta_time);
		//If the next scene exists, load it
		if (loaded_scene->next_scene != nullptr) {
			//Doesnt have a transition? Load immediately
			if (loaded_scene->transition == nullptr) {
				load_scene(loaded_scene->next_scene);
			}
			else { //Does have a transition, use that instead
				//Reset all transition variables
				changing_scene = true;
				detransitioning = false;
				transition_time = 0.0f;
				transition_time_full = 0.0f;
				transition = loaded_scene->transition;
			}
		}
	}
	else {
		//Is the transition is in the first half?
		if (!detransitioning) {
			//Increase the transition time
			transition_time += delta_time * transition->speed;
			transition_time_full += delta_time * transition->speed;
			if (transition_time >= 1.0f) {
				//Reached the second half, load the scene and proceed
				transition_time = 1.0f;
				detransitioning = true;
				just_loaded = true;
				load_scene(loaded_scene->next_scene);
			}
		}
		else {
			//Transition going from 1 -> 0
			if (!just_loaded) {
				transition_time -= delta_time * transition->speed;
				transition_time_full += delta_time * transition->speed;
			}
				
			else just_loaded = false;
			if (transition_time <= 0.0f) {
				transition_time = 0.0f;
				transition_time_full = 0.0f;
				detransitioning = false;
				changing_scene = false;
				transition = nullptr;
			}
			//Should still update the scene on this side of the transition
			loaded_scene->update(delta_time);
		}
	}
}

//Render the scene
void SceneManager::render() {
	loaded_scene->render();
	
}

void SceneManager::post_render() {
	if (changing_scene && transition != nullptr) {
		transition->shader->blend = transition_time;
		transition->shader->blend_full = transition_time_full;
		transition->shader->time = c_time;
		transition->shader->use();
		transition->shader->upload_uniforms();
		glBindVertexArray(square_vao);
		glEnableVertexAttribArray(0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
}