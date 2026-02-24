#pragma once

#include "config.h"
#include "soloud.h"
#include "soloud_wav.h"

class SoundManager {
public:
	SoundManager();
	~SoundManager();
	//Runs every frame to update audio
	void update();
	//Start playing music
	void play_music(SoLoud::Wav* song, double loop_point, float volume);
	//Fade out music then stop it
	void stop_music(bool fade);
	//Play a one-shot sound
	SoLoud::handle play_sound(SoLoud::Wav* sound);
	SoLoud::handle play_sound(SoLoud::Wav* sound, float volume, float panning);
	SoLoud::Soloud soloud;
	float music_volume = 1.0f;
	
private:
	//Needs a reference to the music so it can stop it or change it
	SoLoud::handle music_handle = NULL;
	bool fading = false;
	float current_music_vol = 0.0f;
};
