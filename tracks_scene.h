#pragma once

#include "config.h"
#include "scene.h"
#include "entity.h"
#include "gui_button.h"

class TracksScene : public Scene {
public:
	TracksScene(GUIManager* gui, EntityManager* entities, Toolbox* toolbox);
	~TracksScene();

	void update(float delta_time) override;
	void render() override;
	void start() override;
private:
	GUIButton* start_button;
	GUIButton* back_button;
	GUIImage* track_image;
	GUIButton* track_name;
	int selected_track = 0;
};