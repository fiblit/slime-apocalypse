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
#include "Object.hpp"
#include "Scenario.hpp"

/* GLOBALS */
/* GL */
#pragma warning(push, 0)
#include <glad/glad.h>// glad: an OpenGL function loader: https://github.com/Dav1dde/glad
#include <GLFW/glfw3.h>// glfw: a library for I/O and OpenGL context creation: http://www.glfw.org/
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
#include <iostream>
#include <cstdlib>
#include <string>
#include <chrono>
#include <thread>
#pragma warning(pop)

GLFWwindow * init_window_context();

/* Globals */
namespace G {
	const GLint WIN_WIDTH = 1200;
	const GLint WIN_HEIGHT = 800;
	const GLboolean WIN_FULLSCREEN = GL_FALSE;
}
namespace obj {//should be in G
	GLfloat cube[] = {
		// Positions          // Normals           // Texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};
	GLuint NR_CUBES = 0;
	glm::vec3 * cube_specular_color;
	glm::vec3 * cube_diffuse_color;
	GLfloat * cube_scale;
	glm::vec3 * cube_positions;

    /* These are the multiple box meshes that make up my cylinders */
	GLuint NR_RECT_IN_CIRC = 0;
	GLfloat * rect_in_circ_scale;
	glm::vec4 * rect_in_circ_rotation;
	glm::vec3 * rect_in_circ_positions;

	GLuint NR_RECT = 0;
	glm::vec2 * rect_scale;
	glm::vec3 * rect_positions;

	GLuint NR_AGENT_TO_DRAW = 0;
	GLfloat * agent_scale;
	glm::vec4 * agent_rotation;
	glm::vec3 * agent_positions;
}

Gtime::Timer * game_loop_clock;

GLboolean is_flashlight_on;
void toggle_flashlight();
void handle_input(GLfloat dt);


void move_player(GLfloat dx, GLfloat dy, GLfloat dt);

//AI?
void animate_agents(std::vector<Object *> agents, GLfloat dt);
/* TO be moved to AI manager class */
namespace ai {
    Cspace2D * std_cspace;
    PRM * std_prm;
    //todo dalton: deprecate the following three in favor of spatial struct
    std::vector<Object *> std_NNai;
    std::vector<Object *> std_NNboids;
    std::vector<Object *> std_NNstatic;
    void init(std::vector<BoundingVolume *> bv, std::vector<Object *> NNai, std::vector<Object *> NNboids, std::vector<Object *> NNstatic);
    void replan(std::vector<Object *> agents, Gtime::Timer * clock);
    void calc_LMP_force(Object * a, std::vector<Object *> NNai, std::vector<Object *> NNboids, std::vector<Object *> NNstatic, Cspace2D * a_cspace, GLfloat dt);
};
/* Above move to AI mngr */

#endif //MAIN
