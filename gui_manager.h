#pragma once
#include "config.h"
#include "gui_shader.h"
#include "gui_object.h"
#include "controller.h"
#include "gui_image.h"
#include "toolbox.h"

class GUIManager {
public:
	GUIManager(Toolbox* toolbox);
	~GUIManager();
	void update(float delta_time);
	//Renders all GUI objects
	void render();
	//Renders top layer like cursors
	void late_render();
	void render_one(GUIObject* obj);
	void setup_render_batch();
	void end_render_batch();
	GUIObject* add_object(GUIObject* obj);
	void remove_object(GUIObject* obj);
	void clear();
	gvar square_vao;
	float aspect=ASPECT;
	bool show_cursors = true;
private:
	//Vertices for a square
	GLfloat square[8] = {
//	    x1  y1  x2  y2 x3 y3  x4 y4
		-1, 1, -1, -1, 1, 1, 1, -1
	};
	GUIShader* shader;
	GUIImage* cursor;
	std::vector<GUIObject*> objects;
	Toolbox* toolbox;
};