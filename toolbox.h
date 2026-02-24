#pragma once

#include "config.h"
#include <map>

#include "shader_program.h"
#include "particle_template.h"
#include "particle_shader.h"
#include "camera.h"
#include "renderable.h"
#include "textured_model.h"
#include "obj_loader.h"
#include "font.h"
#include "transition.h"
#include "character.h"

#include "sound_manager.h"
#include "soloud_wav.h"
#include "gamestate.h"

#include "track.h"

#include "sky_shader.h"

#include "settings.h"

//All the resources for the game
class Toolbox {
public:
	Camera* camera;
	GameState state;
	Settings settings;
	std::map<std::string, ParticleTemplate*> particle_templates;
	std::map<std::string, ShaderProgram*> shaders;
	std::map<std::string, Texture*> textures;
	std::map<std::string, Texture*> gui;
	std::map<std::string, Renderable*> renderables;
	std::map<std::string, Model*> models;
	std::map<std::string, Font*> fonts;
	std::map<std::string, Texture*> skies;

	std::map<std::string, Transition*> transitions;

	std::vector<GenericController*> controllers;

	std::vector<Character*> characters;

	std::vector<SoLoud::Wav*> music;
	std::vector<SoLoud::Wav*> ui_sounds;
	std::vector<SoLoud::Wav*> sounds;

	std::vector<Track*> tracks;

	SoundManager* sound_manager;

	gvar square_vao;

	void load_particle_templates() {
		using namespace std;
		//All the different types of particle systems
		particle_templates.insert(pair<string, ParticleTemplate*>("exhaust_smoke",
			new ParticleTemplate
			{ 200,glm::vec3(0,3,0),true,0.3f,1.5f,0.002f,1,0.2f,false,0.6f,true,0.1f,0.0f,
				new Texture("particles/smoke_static",GL_LINEAR),1,4 }
		));
	}
	
	//Adds all the different tracks
	void load_tracks() {
		tracks.push_back(new Track("RAINBOW ROAD",
			"testtrack", 15.0f, skies["space"],
			{ "rainbow","star","line","starbit" },
			glm::vec3(0.5f, 0, -5),
			music[1], 11.734f));
		tracks.push_back(new Track(
			"SWEET TRACK", "sweet",
			15.0f, skies["sweet"], 
			{"road","donut","lolly","lolly2","line","boost"},
			glm::vec3(0.5f,-0.5f,0),
			music[3], 5.331, glm::vec2(0.08f, 0.0f)));

	}
	//Loads all the shader programs
	void load_shaders(){
		using namespace std;
		shaders.insert(pair<string, ShaderProgram*>("particle", new ParticleShader(camera)));
		shaders.insert(pair<string, ShaderProgram*>("sky", new SkyShader(camera)));

	}
	//Load in all the shared textures and sky textures
	void load_textures() {
		using namespace std;
		textures.insert(pair<string, Texture*>("car1", new Texture("karts/car1", GL_LINEAR)));
		textures.insert(pair<string, Texture*>("car2", new Texture("karts/car2", GL_LINEAR)));
		textures.insert(pair<string, Texture*>("car3", new Texture("karts/car3", GL_LINEAR)));
		textures.insert(pair<string, Texture*>("car4", new Texture("karts/car4", GL_LINEAR)));
		
		textures.insert(pair<string, Texture*>("xaverian", new Texture("Xaverian", GL_LINEAR)));

		skies.insert(pair<string, Texture*>("space", new Texture("sky/space", GL_LINEAR)));
		skies.insert(pair<string, Texture*>("sweet", new Texture("sky/sweet", GL_LINEAR)));
		skies.insert(pair<string, Texture*>("normal", new Texture("sky/normal", GL_LINEAR)));
	}
	//Loads in all the basic meshes
	void load_models() {
		using namespace std;
		models.insert(pair<string, Model*>("kart1", OBJLoader::loadOBJ("karts/kart1")));
		models.insert(pair<string, Model*>("sky", OBJLoader::loadOBJ("sky")));
		models.insert(pair<string, Model*>("cat", OBJLoader::loadOBJ("characters/cat")));
		models.insert(pair<string, Model*>("duke", OBJLoader::loadOBJ("characters/duke")));
		models.insert(pair<string, Model*>("robot", OBJLoader::loadOBJ("characters/robot")));
		models.insert(pair<string, Model*>("cube", OBJLoader::loadOBJ("cube")));
	}
	//Loads in all the GUI elements as textures
	void load_gui_elements() {
		using namespace std;
		gui.insert(pair<string, Texture*>("top_bar", 
		new Texture("gui/title", GL_LINEAR)));
		gui.insert(pair<string, Texture*>("slider",
		 new Texture("gui/slider", GL_LINEAR)));
		gui.insert(pair<string, Texture*>("knob",
		 new Texture("gui/knob", GL_LINEAR)));
		gui.insert(pair<string, Texture*>("cogholder",
		 new Texture("options/cog_holder", GL_LINEAR)));
		gui.insert(pair<string, Texture*>("box",
		 new Texture("gui/box", GL_LINEAR)));
		gui.insert(pair<string, Texture*>("main/one_player",
		 new Texture("gui/main/one_player", GL_LINEAR)));
		gui.insert(pair<string, Texture*>("main/player_button",
		 new Texture("gui/main/players", GL_LINEAR)));
		gui.insert(pair<string, Texture*>("main/settings",
		 new Texture("gui/main/settings", GL_LINEAR)));
		gui.insert(pair<string, Texture*>("main/stats", 
		new Texture("gui/main/stats", GL_LINEAR)));
		gui.insert(pair<string, Texture*>("main/quit", 
		new Texture("gui/main/quit", GL_LINEAR)));
		gui.insert(pair<string, Texture*>("character/player_box", 
		new Texture("gui/character/player_box", GL_LINEAR)));
		gui.insert(pair<string, Texture*>("character/characters", 
		new Texture("gui/character/icons", GL_LINEAR)));
		gui.insert(pair<string, Texture*>("back_button", 
		new Texture("gui/back_button", GL_LINEAR)));

		gui.insert(pair<string, Texture*>("dividery", new Texture("gui/divider", GL_LINEAR)));
		gui.insert(pair<string, Texture*>("dividerx", new Texture("gui/divider2", GL_LINEAR)));

		gui.insert(pair<string, Texture*>("carmap", new Texture("gui/carmap", GL_LINEAR)));

		gui.insert(pair<string, Texture*>("places", new Texture("gui/places", GL_LINEAR)));
		gui.insert(pair<string, Texture*>("laps", new Texture("gui/laps", GL_LINEAR)));

		gui.insert(pair<string, Texture*>("countdown", 
		new Texture("gui/countdown", GL_LINEAR)));

		gui.insert(pair<string, Texture*>("congratulations",
		 new Texture("gui/congratulations", GL_LINEAR)));

		gui.insert(pair<string, Texture*>("leaderboard_item",
		new Texture("gui/leaderboard_item", GL_LINEAR)));

	}
	//Loads in all the more complex models as renderables, usually textured or multitexture models
	void load_renderables() {
		using namespace std;
		renderables.insert(pair<string, TexturedModel*>("logo", 
		new TexturedModel(OBJLoader::loadOBJ("logo"), textures["car1"])));
		renderables.insert(pair<string, TexturedModel*>("cog", 
		new TexturedModel(OBJLoader::loadOBJ("options/cog"), new Texture("options/cog", GL_LINEAR))));

		renderables.insert(pair<string, TexturedModel*>("kart1", 
		new TexturedModel(models["kart1"], textures["car1"])));
		renderables.insert(pair<string, TexturedModel*>("kart1skin2", 
		new TexturedModel(models["kart1"], textures["car2"])));
		renderables.insert(pair<string, TexturedModel*>("kart1skin3", 
		new TexturedModel(models["kart1"], textures["car3"])));
		renderables.insert(pair<string, TexturedModel*>("kart1skin4", 
		new TexturedModel(models["kart1"], textures["car4"])));
	
		renderables.insert(pair<string, TexturedModel*>("wheel", new 
		TexturedModel(OBJLoader::loadOBJ("karts/wheel"), textures["car1"])));
		renderables.insert(pair<string, TexturedModel*>("steeringwheel", 
		new TexturedModel(OBJLoader::loadOBJ("karts/steeringwheel"), textures["car1"])));
		
		renderables.insert(pair<string, MultitextureModel*>("podium", 
		new MultitextureModel(OBJLoader::loadOBJ("podium/podium"), {
			new Texture("podium/podiums",GL_LINEAR),new Texture("podium/numbers",GL_LINEAR) })));
		renderables.insert(pair<string, MultitextureModel*>("castle", 
		new MultitextureModel(OBJLoader::loadOBJ("podium/castle"), 
			{	new Texture("podium/grass",GL_LINEAR),
				new Texture("podium/sand",GL_LINEAR),
				new Texture("podium/roof",GL_LINEAR),
				new Texture("podium/wall",GL_LINEAR),
				new Texture("podium/path",GL_LINEAR),
				new Texture("podium/bricks",GL_LINEAR),
				new Texture("podium/Tree",GL_LINEAR),
				new Texture("podium/Window",GL_LINEAR),
				new Texture("podium/Window2",GL_LINEAR),
				new Texture("podium/Flag",GL_LINEAR) 
			})));

	}

	//No other fonts actually used
	void load_fonts() {
		using namespace std;

	}
	//All the transitions used between scenes, actually just shaders with a speed variable
	void load_transitions() {
		using namespace std;

		transitions.insert(pair<string, Transition*>("pixel", new Transition("pixeltransition", 2.0f)));
		transitions.insert(pair<string, Transition*>("zoom", new Transition("zoomtransition", 1.0f)));
		transitions.insert(pair<string, Transition*>("swipe", new Transition("swipetransition", 1.0f)));

		transitions.insert(pair<string, Transition*>("respawn", new Transition("respawntransition", 2.0f)));

	}
	//All the different characters and their models and data
	void load_characters() {
		using namespace std;
		characters.push_back(new Character("cat1", "CAT 1   ", CharacterType::CAT1,
		new TexturedModel(models["cat"],new Texture("characters/cat1",GL_LINEAR))));
		characters.push_back(new Character("cat2", "CAT 2   ", CharacterType::CAT2, 
		new TexturedModel(models["cat"], new Texture("characters/cat2", GL_LINEAR))));
		characters.push_back(new Character("cat3", "CAT 3   ", CharacterType::CAT3, 
		new TexturedModel(models["cat"], new Texture("characters/cat3", GL_LINEAR))));
		characters.push_back(new Character("Duke1", "DUKE 1  ", CharacterType::DUKE1, 
		new TexturedModel(models["duke"], new Texture("characters/duke1", GL_LINEAR))));
		characters.push_back(new Character("Duke 2", "DUKE 2  ", CharacterType::DUKE2, 
		new TexturedModel(models["duke"], new Texture("characters/duke2", GL_LINEAR))));
		characters.push_back(new Character("Duke 3", "DUKE 3  ", CharacterType::DUKE3,
		 new TexturedModel(models["duke"], new Texture("characters/duke3", GL_LINEAR))));
		characters.push_back(new Character("Robot", "ROBOT   ", CharacterType::ROBOT, 
		new TexturedModel(models["robot"], new Texture("characters/robot", GL_NEAREST))));

	}
	//All the different sounds in the game
	void load_sounds() {
		using namespace std;
		load_music("menu");
		load_music("testtrack");
		load_music("options");
		load_music("sweet");
		load_music("podium");
		load_music("leaderboard");

		load_ui_sound("button1");
		load_ui_sound("button2");
		load_ui_sound("button3");
		load_ui_sound("buttonback");

		load_game_sound("cars/engine_loop");
		load_game_sound("fall");
		load_game_sound("intro");
	}
	//Need to initialise enough to show the loading scene
	void init(Camera* cam) {
		using namespace std;
		this->camera = cam;
		sound_manager = new SoundManager();
		fonts.insert(pair<string, Font*>("bold", new Font("bold", 45)));
		gui.insert(pair<string, Texture*>("square", new Texture("gui/square", GL_NEAREST)));
		
		//Loads in a square to render the loading scene
		gvar vbo_id;
		glGenBuffers(1, &vbo_id);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);

		glGenVertexArrays(1, &square_vao);
		glBindVertexArray(square_vao);

		glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), 0);

		glEnableVertexAttribArray(0);

		glBindVertexArray(0);

		settings.init(sound_manager);

	}
	//Loads in music from "./res/sounds/music/NAME.wav"
	bool load_music(std::string name) {

		std::cout << "\033[33mLoading music: \033[36m" << name << "\033[37m\n";
		music.push_back(new SoLoud::Wav());
		if (music.back()->load(("./res/sounds/music/" + name + ".wav").c_str())!=0) {
			std::cout << "Error loading song: " << name << "!\n";
			return false;
		}

		return true;
	}
	//Loads in a sound the same was as load music but with a different path
	bool load_ui_sound(std::string name) {

		std::cout << "\033[33mLoading UI sound: \033[36m" << name << "\033[37m\n";
		ui_sounds.push_back(new SoLoud::Wav());
		if (ui_sounds.back()->load(("./res/sounds/menu/" + name + ".wav").c_str())!=0) {
			std::cout << "Error loading UI sound: " << name << "!\n";
			return false;
		}

		return true;
	}
	//Loads in a sound the same was as load music but with a different path
	bool load_game_sound(std::string name) {
		std::cout << "\033[33mLoading game sound: \033[36m" << name << "\033[37m\n";
		sounds.push_back(new SoLoud::Wav());
		if (sounds.back()->load(("./res/sounds/game/" + name + ".wav").c_str()) != 0) {
			std::cout << "Error loading game sound: " << name << "!\n";
			return false;
		}

		return true;
	}
private:
	//The vertices for a square
	GLfloat square[8] = {
		//	    x1  y1  x2  y2 x3 y3  x4 y4
				-1, 1, -1, -1, 1, 1, 1, -1
	};
};