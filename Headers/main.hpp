#ifndef MAIN
#define MAIN
#pragma once

/* LOCALS */
/* MACROS */
#include "debug.hpp"

/* CLASSES */
#include "Shader.hpp"
#include "Camera.hpp"
#include "Timer.hpp"
#include "PRM.hpp"
#include "GlobalMotionPlanner.hpp"
#include "LocalMotionPlanner.hpp"
#include "Agent.hpp"
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
namespace mouse {
	GLfloat last_x = G::WIN_WIDTH / 0.5f;
	GLfloat last_y = G::WIN_HEIGHT / 0.5f;
	GLboolean focus = false;
	//GLfloat sensitivity = 0.25f;
}

Camera* cam;
G::time::Timer * game_loop_clock;

const GLuint cylinder_res = 11;

GLuint cur_mode;
BoundingVolume * cur_ob;

Circ * player;
std::vector<Agent *> boidlings;

namespace G { const GLboolean WITH_TTC_GRID = GL_TRUE; }
std::vector<Agent *> agents_old;
int NUM_AGENTS = 0;
std::vector<Agent *> agents[100][100];

std::vector<Rect *> rect_bounds;
std::vector<Circ *> obst_bounds;

GLboolean is_flashlight_on;
int selected_agent_debug = 0;

/* UI prototypes */
bool keys[1024];
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement();

void toggle_flashlight();

void init_planning();
void init_planning_vis();
void replan();
void place_obst(glm::vec3 pos);
void place_goal_node(glm::vec3 pos);
void place_start_node(glm::vec3 pos);
void mode_toggle_current_obstacle();
void scale_current_obstacle(GLfloat xs, GLfloat ys, GLfloat dt);
void move_current_obstacle(GLfloat xs, GLfloat ys, GLfloat dt);

void move_player(GLfloat dx, GLfloat dy, GLfloat dt);

int kill_app(int retVal);
void animate_agents(GLfloat dt);

#endif //MAIN
