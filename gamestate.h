#pragma once

#include "track.h"

struct GameState {
public:
	//How many human racers there are
	int num_players = 1;
	//Total racers
	int num_racers = 8;
	//Current track to be loaded, used for tournaments
	int cTrack = 0;
	//What characters everyone has selected, mostly
	//	redundant as CPUs are forced to use the robot (8)
	int player_characters[8] = { 0,1,2,3,5,6,7,8 };
	//places[0] is what racer got 1st place etc...
	int places[8] = {0,1,2,3,4,5,6,7};
	//What score each racer is on for tournaments
	int scores[8] = { 0,0,0,0,0,0,0,0 };
	//Currently loaded track
	Track* current_track = nullptr;
};