#pragma once
#include "config.h"
#include "sound_manager.h"
#include <filesystem>

struct Settings {
public:
	//setting values
	float music_volume = 1.0f;
	float master_volume = 1.0f;
	bool cpus = true;
	bool tournament = true;

	void init(SoundManager* sound);
	void update();
	void write();
private:
	SoundManager* sound;
	std::filesystem::path path;
};