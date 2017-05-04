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
#include "Agent.hpp"
#include "Scenario.hpp"
#include "Scene.hpp"
#include "Structs.hpp"

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
// moved the namespaces to Structs.hpp so that Scene could access them

G::time::Timer * game_loop_clock;

const GLuint cylinder_res = 11;

GLuint cur_mode;
BoundingVolume * cur_ob;

Scene * scene;

Circ * player;
std::vector<Agent *> boidlings;

namespace G { const GLboolean WITH_TTC_GRID = GL_TRUE; }
std::vector<Agent *> agents_old;
int NUM_AGENTS = 0;
std::vector<Agent *> agents[100][100];

std::vector<Rect *> rect_bounds;
std::vector<Circ *> obst_bounds;

void inc_NR_AGENT_TO_DRAW(std::vector<Agent *> agents);
void count_NR_AGENT_TO_DRAW();//ttc_grid_branch(^^)

void update_agents_bin(std::vector<Agent *> agents, GLuint * offset);
void init_agent_data();//ttc_grid_branch(^^)

void handle_input(GLfloat dt);

GLboolean is_flashlight_on;
void toggle_flashlight();

void init_planning();
GLint selected_agent_debug = 0;
void init_planning_vis();
void replan();

void place_obst(glm::vec3 pos);
void place_goal_node(glm::vec3 pos);
void place_start_node(glm::vec3 pos);
void mode_toggle_current_obstacle();
void scale_current_obstacle(GLfloat xs, GLfloat ys, GLfloat dt);
void move_current_obstacle(GLfloat xs, GLfloat ys, GLfloat dt);

void move_player(GLfloat dx, GLfloat dy, GLfloat dt);

void animate_agents(GLfloat dt);

#endif //MAIN
