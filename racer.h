#pragma once

#include "config.h"
#include "entity.h"
#include "generic_controller.h"
#include "toolbox.h"
#include "particle_system.h"
#include "sound_manager.h"
#include "track.h"

class Racer : public Entity {
public:
	Racer(glm::vec3 position, Toolbox* toolbox, Camera* camera, int place,bool human);
	~Racer();
	void update(float delta_time) override;
	void render() override {}
	Entity* camera_pos;
	//Variables to smooth the camera movement so it looks less rigid
	glm::vec3 camera_smooth_pos;
	glm::quat camera_smooth_rot;
	//Cars velocity is split between different effectors
	glm::vec3 velocity;
	glm::vec3 hit_velocity;
	//When was the car last hit?
	float last_hit;

	//Respawning variables to help with the transition in game_scene.cpp
	float respawn_blend_full;
	float respawn_blend;
	bool respawning;

	//Ended the race yet? if so in what place
	bool finished = false;
	int final_place = 0;

	//Player 1, 2, 3, etc...
	int place;

	//Racers are frozen at the start of the race
	bool frozen = true;
	
	//Keeping track of progress in the race
	int current_place = 0;
	int current_checkpoint = 0;
	int current_lap = 0;
	float checkpoint_distance = 0;

	//Allows for input from different racer types (Human and CPU racers)
	bool should_accelerate = false;
	bool should_drift = false;
	float left_stick = 0.0f;
	//Allows for tweaking speed depending on if they are a CPU or are human
	bool human = true;
protected:
	Toolbox* toolbox;
	float rot_y;
private:
	//Rotates wheels over time with velocity
	void rotate_wheels(float amount, float turn);
	//How fast the car is going
	float acceleration;
	//Going left instantly doesnt instantly turn the car
	float dampened_left_stick=0.0f;
	SoundManager* sound;
	float pan;
	float respawn_time;
	int start_ticks = 0;
	//Smoke
	ParticleSystem* p1;
	ParticleSystem* p2;
	//Engine sound handle so I can change pitch with velocity
	SoLoud::handle engine_sound;
	glm::vec3 start_pos;
	glm::vec3 true_pos;
	
	//Variables for tracking falling
	float leave_ground_time;
	float time;
	glm::vec3 last_track_normal;
	Track* track;
	float y_vel = 0.0f;
};