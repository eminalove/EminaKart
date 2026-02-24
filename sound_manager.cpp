#include "sound_manager.h"

SoundManager::SoundManager() {
	soloud.init();
}
//Play some music :)
void SoundManager::play_music(SoLoud::Wav* song,
	double loop_point,
	float volume) {
	//Is the music already playing? Stop it
	if (music_handle != NULL) {
		soloud.stop(music_handle);
	}
	//Play the song and get the handle
	music_handle = soloud.play(*song,volume*music_volume);
	//Set the loop point and volume
	soloud.setLoopPoint(music_handle, loop_point);
	soloud.setLooping(music_handle,true);
	soloud.setVolume(music_handle, volume*music_volume);
	current_music_vol = volume;

}
void SoundManager::stop_music(bool fade) {
	//Stops the music and can also fade it out
	if (fade)
		soloud.fadeVolume(music_handle, 0, 0.2f);
	else {
		soloud.stop(music_handle);
		music_handle = NULL;
	}
	fading = fade;
}
//Plays a sound and returns a handle
SoLoud::handle SoundManager::play_sound(SoLoud::Wav* sound) {
	return soloud.play(*sound);
}SoLoud::handle SoundManager::play_sound(SoLoud::Wav* sound, float volume, float panning) {
	SoLoud::handle h = soloud.play(*sound);
	soloud.setVolume(h, volume);
	soloud.setPan(h, panning);
	return h;
}
//Updates fading and 3D audio
void SoundManager::update() {
	soloud.update3dAudio();
	if (fading) {
		//Stop music if its faded enough
		if (soloud.getVolume(music_handle) <= 0.001f) {
			fading = false;
			soloud.stop(music_handle);
			music_handle = NULL;
		}
	}
	else {
		soloud.setVolume(music_handle, current_music_vol * music_volume);
	}
	
}

SoundManager::~SoundManager() {
	soloud.deinit();
}