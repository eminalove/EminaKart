#include "gui_manager.h"

GUIManager::GUIManager(Toolbox* toolbox) {
	shader = new GUIShader();
	this->toolbox = toolbox;
	
	//Image containing all of the cursors
	cursor = new GUIImage(new Texture("gui/cursors", GL_LINEAR), glm::vec2(0), glm::vec2(1));
	cursor->num_sprites = 4;
	
	//Load in a square mesh
	gvar vbo_id;
	glGenBuffers(1, &vbo_id);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);

	glGenVertexArrays(1, &square_vao);
	glBindVertexArray(square_vao);

	glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), 0);

	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}
//Clean up
GUIManager::~GUIManager() {
	glDeleteVertexArrays(1,&square_vao);
}
void GUIManager::setup_render_batch() {
	//Set up a render batch for rendering GUI components
	shader->use();
	shader->aspect = aspect;
	glBindVertexArray(square_vao);
	glEnableVertexAttribArray(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}
//Clean up after render
void GUIManager::end_render_batch() {

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}
//Render within a render batch
void GUIManager::render_one(GUIObject* obj) {
	obj->render(square_vao, shader);
}
//Renders all the GUI components
void GUIManager::render() {
	
	shader->use();
	shader->aspect = aspect;
	glBindVertexArray(square_vao);
	glEnableVertexAttribArray(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->render(square_vao, shader);
	}

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	
}
//Draws everything that is on top
void GUIManager::late_render() {
	shader->use();
	shader->aspect = aspect;
	glBindVertexArray(square_vao);
	glEnableVertexAttribArray(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	//Draw cursors
	if (show_cursors) {
		for (int i = 0; i < 4; i++) {
			//Is the controller connected?
			if (toolbox->controllers[i]->present) {
				//Move the cursor object to the controllers cursor position
				cursor->position = toolbox->controllers[i]->cursor_position - glm::vec2(cursor->scale.x / 2.0f);
				//Player 1-4
				cursor->current_sprite = i;
				//Allow for changing the opacity
				cursor->colour.a = toolbox->controllers[i]->cursor_opacity;
				//Draw the cursor
				cursor->render(square_vao, shader);
			}
		}
	}

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}
void GUIManager::update(float delta_time) {
	if (show_cursors) {
		for (int i = 0; i < 4; i++) {

			toolbox->controllers[i]->hovering = false;
			if (toolbox->controllers[i]->present) {
				glm::vec2& pos = toolbox->controllers[i]->cursor_position;
				//Move the cursor using the left stick
				pos += toolbox->controllers[i]->left_stick * glm::vec2(10,-10) * delta_time;
				//Clamp cursor on the screen
				pos = glm::clamp(pos,glm::vec2(	-WIDTH_UNITS+1.4f,
												-HEIGHT_UNITS-0.2f), 
												glm::vec2(WIDTH_UNITS+0.5f, 
												HEIGHT_UNITS-0.5f));
				
				float closest_distance = 100.0f;
				int closest_index = -1;

				//Get the closest cursor
				for (int j = 0; j < 4; j++) {
					if (i!=j && toolbox->controllers[j]->present) {
						float distance = glm::distance(pos, toolbox->controllers[j]->cursor_position);
						if (distance < closest_distance) {
							closest_distance = distance;
							closest_index = j;
						}
					}
				}
				//Fade if cursors are close
				toolbox->controllers[i]->cursor_opacity = 1.0f;
				if (closest_index != -1) {
					if (closest_distance < 2.0f) {
						toolbox->controllers[i]->cursor_opacity = (closest_distance/4.0+0.5f);
					}
				}

			}
		}
	}
	//Make sure objects know if they are being hovered over by using a simple
	//rectangle vs point collision
	for (int i = 0; i < objects.size(); i++) {
		if (show_cursors) {
			objects[i]->hovering = 0;
			for (int j = 0; j < 4; j++) {
				if (toolbox->controllers[j]->present &&
					!toolbox->controllers[j]->hovering) {
					glm::vec2 pos = toolbox->controllers[j]->cursor_position+glm::vec2(-1.5f,0.5f);
					glm::vec2 pos2 = objects[i]->position;
					if (pos.x >= pos2.x - objects[i]->scale.x &&
						pos.x <= pos2.x + objects[i]->scale.x &&
						pos.y >= pos2.y - objects[i]->scale.y &&
						pos.y <= pos2.y + objects[i]->scale.y) {
						objects[i]->hovering |= (1u << j);

						toolbox->controllers[j]->hovering = true;
					}
				}
			}
		}
		objects[i]->update(delta_time);
	}
}
GUIObject* GUIManager::add_object(GUIObject* obj) {
	objects.push_back(obj);
	return obj;
}
//Find the object and remove it
void GUIManager::remove_object(GUIObject* obj) {
	for (int i = 0; i < objects.size(); i++) {
		//Are the pointers the same?
		if (objects[i] == obj) {
			//Remove from the objects list and delete the object
			objects.erase(objects.begin() + i);
			delete obj;
			return;
		}
	}
}
void GUIManager::clear() {
	for (int i = 0; i < objects.size(); i++) {
		delete objects[i];
	}
	objects.clear();
}