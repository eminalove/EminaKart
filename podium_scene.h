#pragma once
#include "config.h"
#include "scene.h"
#include "entity.h"
#include "gui_button.h"
#include "racer.h"
#include "track.h"
#include "sky_shader.h"
#include "gui_image.h"
#include "model.h"
#include "soloud.h"

class PodiumScene : public Scene {
public:
	PodiumScene(GUIManager* gui, EntityManager* entities, Toolbox* toolbox);
	~PodiumScene();

	void update(float delta_time) override;
	void render() override;
	void start() override;
private:
	//Shader for rendering the sky
	SkyShader* sky_shader;
	//Sphere mesh with a sky texture
	TexturedModel* sky;
	//All the characters on the podium
	std::vector<Entity*> chars;
	float time;
	//Congratulations!!! image
	GUIImage* congrats;
};