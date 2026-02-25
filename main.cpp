#include "config.h"
#include "entity_manager.h"
#include "camera.h"
#include "gui_manager.h"
#include <chrono>

#include "scene_manager.h"
#include "loading_scene.h"
#include "podium_scene.h"

#include "controller.h"
#include "keyboard_controller.h"

#include "toolbox.h"


int main() {
	std::cout << "Emina Kart\n";

	GLFWwindow* window;
	
	//Initialise GLFW and exit if it hasn't been initialised correctly
	if (!glfwInit()) {
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create a window
	window = glfwCreateWindow(WIDTH, HEIGHT, "Emina Kart", NULL, NULL);
	glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
	glfwMakeContextCurrent(window);

	//Exit if there is an error loading OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Couldn't load opengl\n";
		glfwTerminate();
		return -1;
	}

	Toolbox* toolbox = new Toolbox();

	//The colour the window should display when nothing is drawn there
	glClearColor(1, 0.824, 0.984, 1.0f);

	Camera* cam = new Camera(glm::vec3(0, 0, 0));

	EntityManager* entity_manager = new EntityManager(cam);

	toolbox->init(cam);

	KeyboardController* p1 = new KeyboardController(window);
	Controller* p2 = new Controller(GLFW_JOYSTICK_1);
	Controller* p3 = new Controller(GLFW_JOYSTICK_2);
	Controller* p4 = new Controller(GLFW_JOYSTICK_3);

	toolbox->controllers.push_back(p1);
	toolbox->controllers.push_back(p2);
	toolbox->controllers.push_back(p3);
	toolbox->controllers.push_back(p4);
	

	GUIManager* gui = new GUIManager(toolbox);
	
	SceneManager* scene_manager = new SceneManager(new LoadingScene(gui, entity_manager, toolbox),toolbox);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glViewport(0, 0, WIDTH, HEIGHT);
	//Time of the last frame
	auto last_time = std::chrono::steady_clock::now();

	//When did the program start
	auto start_time = last_time;
	using namespace std::chrono_literals;
	float time = 0;
	
	//Loop until the user tries to close the window
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		toolbox->sound_manager->update();
		p1->update();
		p2->update();
		p3->update();
		p4->update();

		//Get the current time of the frame
		auto current_time = std::chrono::steady_clock::now();
		//How long since the last frame
		auto elapsed = current_time - last_time;
		//Get the delta time in seconds from nanoseconds (divide by 1 billion)
		double delta_time = elapsed.count() / 1000000000.0;
		//Refresh the last time for next frame
		last_time = std::chrono::steady_clock::now();

		time += delta_time;
		

		cam->update_camera();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if(glfwGetKey(window,GLFW_KEY_L)==GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//Clear the colour and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		scene_manager->update(delta_time);
		scene_manager->render();
		scene_manager->post_render();

		//Puts whats in the colour buffer onto the window, 
		//  and whats on the window into the colour buffer
		glfwSwapBuffers(window);
	}

	//Clean everything up at the end

	delete entity_manager;
	delete gui;
	delete toolbox->sound_manager;
	glfwTerminate();

	return 0;
}