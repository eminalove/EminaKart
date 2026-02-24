#pragma once
#include "racer.h"

//Implementation of the Racer class
class CPURacer : public Racer {
public:
	CPURacer(glm::vec3 position, Toolbox* toolbox, Camera* camera, int place, std::vector<Racer*>* racers);
	void update(float delta_time);
private:
	//Time since created
	float time;
	//Needs to see all the other racers
	std::vector<Racer*>* racers;
};