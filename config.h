#pragma once

//Loads OpenGL
#include <glad/glad.h>

//Window and some input management for OpenGL
#include <GLFW/glfw3.h>

//OpenGL maths classes and functions
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

//I need this to be able to output to the console
#include <iostream>

//Some libraries that are helpful for files
#include <fstream>
#include <sstream>
#include <string>

//Gives some conventient type descriptors for integers
#include <stdint.h>

//Like a list but its c++ so its called a vector I guess
#include <vector>

//Should be precise enough
#define M_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062f

#define WIDTH 1920.0f
#define HEIGHT 1080.0f
#define ASPECT (HEIGHT/WIDTH)

//UI system uses units instead of pixels
#define WIDTH_UNITS (8.0f/ASPECT)
#define HEIGHT_UNITS 8.0f

//gvar is faster than typing unsigned int every time
typedef unsigned int gvar;
//Even though they are the same typedef, it can distinguish between generic 
//GPU variables and variables within a shader 
typedef unsigned int svar;