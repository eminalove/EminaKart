#include "cpu_racer.h"

CPURacer::CPURacer(glm::vec3 position, Toolbox* toolbox, Camera* camera, int place, std::vector<Racer*>* racers) :
	Racer(position, toolbox, camera, place,false) {
	this->racers = racers;
}
void CPURacer::update(float delta_time) {
	__super::update(delta_time);
	//Dont update if the car is frozen
	if (frozen) return;
	time += delta_time;
	should_accelerate = true;
	
	//The bounding box for the next checkpoint
	BoundingBox c = toolbox->state.current_track->checkpoints[(current_checkpoint + 1)
		% toolbox->state.current_track->checkpoints.size()];
	glm::vec2 dest = glm::vec2(c.position.x, c.position.z);
	glm::vec2 pos = glm::vec2(transform.position.x, transform.position.z);

	//Forward vector of the car
	glm::vec2 forward = glm::vec2(cos(glm::radians(rot_y - 90)),
		sin(glm::radians(rot_y - 90)));

	glm::vec2 toPoint = glm::normalize(dest - pos);
	float cross = forward.x * toPoint.y - forward.y * toPoint.x;
	float checkpoint_influence = -cross;
	float avoid_influence = 0.0f;

	//Go through all the racers to avoid them
	for (int i = 0; i < racers->size(); i++) {
		Racer* r = racers->at(i);
		//Is it yourself? skip
		if (r->place == this->place) continue;
		//Is it within 5 units?
		if (glm::distance(r->transform.position, transform.position) < 5) {
			//Steer away
			toPoint = glm::normalize(
				glm::vec2(r->transform.position.x,r->transform.position.z) - pos);

			cross = forward.x * toPoint.y - forward.y * toPoint.x;
			avoid_influence = cross/4.0f;
		}
	}

	left_stick = checkpoint_influence + avoid_influence;
}