#include "settings.h"

//Loads in the settings file
void Settings::init(SoundManager* sound) {
	//Load in the file and create it if it doesnt exist
	this->sound = sound;
	path={ ".\\save" };
	path /= "settings.save";
	std::filesystem::create_directories(path.parent_path());
	std::cout << "\033[33mLoading settings data\033[37m\n";
	if (!std::filesystem::exists(path)) {
		std::ofstream ofs(path);
		ofs << "0.5\n0.5\n1\n0\n";
		ofs.close();
	}
	//Load in the file and parse all its contents
	std::ifstream file(path);
	std::string line;
	std::getline(file, line);
	master_volume = std::stof(line);
	std::getline(file, line);
	music_volume = std::stof(line);
	std::getline(file, line);
	cpus = std::stoi(line)>0;
	std::getline(file, line);
	tournament = std::stoi(line) > 0;
	file.close();
	update();
}
void Settings::update() {
	sound->soloud.setGlobalVolume(master_volume);
	sound->music_volume = music_volume;
}
void Settings::write() {
	//Write all the settings to a file
	std::filesystem::remove(path);
	if (!std::filesystem::exists(path)) {
		std::ofstream ofs(path);
		ofs << std::to_string(master_volume) <<"\n";
		ofs << std::to_string(music_volume) << "\n";
		ofs << (cpus ? 1 : 0) << "\n";
		ofs << (tournament ? 1 : 0) << "\n";
		ofs.close();
	}
}