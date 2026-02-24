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

class GameScene : public Scene {
public:
	GameScene(GUIManager* gui, EntityManager* entities, Toolbox* toolbox);
	~GameScene();

	void update(float delta_time) override;
	void render() override;
	void start() override;
private:
	//All the human and non human racer entities
	std::vector<Racer*> racers;
	//How many human players are there?
	int human_players;
	//In the countdown?
	bool intro = true;
	//Countdown sound
	SoLoud::handle intro_sound;
	//How many racers have finished the race?
	int numFinished = 0;
	//3,2,1 image
	GUIImage* countdown;
	//Current track
	Track* track;
	//Renders a subscreen for a splitscreen
	void render_screen(float w, float h, float x, float y, int racer);
	//Shader for the sky
	SkyShader* sky_shader;
	//A sphere that goes around the map for the sky
	Model* sky;
	//The actual rendered map as an entity, crude but effective
	Entity* track_map;
	//Just a pixel, needed for the map preview model instead of 
	//	making an entirely new shader
	Texture* square_texture;

	//What the racers look like on the map
	GUIImage* car_map;
	//1st, 2nd, 3rd... images
	GUIImage* place;
	//1/3, 2/3, 3/3 images
	GUIImage* lap;
	//The transition that appears when respawning
	Transition* respawn_transition;
	//Time since scene opened
	float time;
	
};