#ifndef MAIN
#define MAIN
#pragma once

/* LOCALS */
#include "debug.hpp"
#include "UI.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Timer.hpp"
#include "PRM.hpp"
#include "GlobalMotionPlanner.hpp"
#include "LocalMotionPlanner.hpp"
#include "AI.hpp"
#include "Object.hpp"
#include "Scenario.hpp"
#include "Scene.hpp"
#include "Structs.hpp"

/* GLOBALS */
/* GL */
#pragma warning(push, 0)
#pragma warning(disable: 4055)
#include <glad/glad.h>// glad: an OpenGL function loader: https://github.com/Dav1dde/glad
#include <GLFW/glfw3.h>// glfw: a library for I/O and OpenGL context creation: http://www.glfw.org/
#pragma warning(disable: 4201)
#include <glm/glm.hpp>// glm: OpenGL mathematics: http://glm.g-truc.net/0.9.8/index.html
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#ifndef STB_IMAGE_IMPLEMENTATION// stb: image loading: https://github.com/nothings/stb/blob/master/stb_image.h
#define STB_IMAGE_IMPLEMENTATION
#endif // STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
//#include <assimp/Importer.hpp>// assimp: model loading: https://github.com/assimp/assimp
//#include <assimp/postprocess.h>
//#include <assimp/scene.h>
/* STL */
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#pragma warning(pop)

GLFWwindow * init_window_context();
int kill_app(int retVal);

void handle_input(Gtime::Timer * clock, Scene * scene);

/* Globals */
Scene * scene;


#endif //MAIN
