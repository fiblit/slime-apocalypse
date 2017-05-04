#ifndef STRUCTS_H_GUARD
#define STRUCTS_H_GUARD

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "graph.hpp"

/*
TODO:
I'm going to split these up into more headers, as sometimes the POD structs/components I make are better
suited with having functions
*/



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
       INDIE plans independently */
    enum class Planner {LEAD, PACK, BOID, INDY};
    Planner method;

    //Instead of Node<glm::vec2> *>, this allows more dynamic path planning
    glm::vec2 goal;

    int num_done;
    //the amount of pointing might cause cache-misses
    std::vector<Node<glm::vec2> *> * plan;

    bool has_plan() {
        return plan != nullptr && plan->size() > static_cast<size_t>(0);
    };
    bool has_boid_f() {
        return method == ai_comp::Planner::BOID || method == ai_comp::Planner::PACK;
    };
    bool has_indy_f() {
        return method == ai_comp::Planner::LEAD || method == ai_comp::Planner::INDY;
    };

    /*
    It might be more convenient for me to have a reference to the cspace somehow (like, in the
    dynamics or collision component). Rather than passing that in as a parameter everywhere.
    The same would be true for the PRM, however, I'll see what I can do without that step.    
    */
};

#endif // STRUCTS_H_GUARD