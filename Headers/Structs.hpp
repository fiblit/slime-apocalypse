#ifndef STRUCTS_H_GUARD
#define STRUCTS_H_GUARD

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "graph.hpp"

struct bufferContainer {
	GLuint scene_vao;
	GLuint position_vbo;
	GLuint colors_vbo;
	GLuint faces_ibo;
	GLuint edges_ibo;
	GLuint normals_vbo;
};

//used by any dynamics module (physics & some local motion planning)
struct dynamics_comp {
    glm::vec3 pos;
    glm::vec3 vel;
    glm::vec3 acc;
    glm::vec3 mass;
    glm::vec3 force;
};

//used by the motion planning module (mostly global MP)
struct ai_comp {
    /* LEADlings attract PACKlings and plan independently
       PACKlings attract PACKlings and BOIDlings
       BOIDlings attract BOIDlings and PACKlings
       INDIE plans independtly */
    enum class Planner {LEAD, PACK, BOID, INDIE};
    Planner method;

    //Instead of Node<glm::vec2> *>, this allows more dynamic path planning
    glm::vec2 goal;

    int completed_nodes;
    //the amount of pointing might cause cache-misses
    std::vector<Node<glm::vec2> *> * plan;
};

#endif // STRUCTS_H_GUARD