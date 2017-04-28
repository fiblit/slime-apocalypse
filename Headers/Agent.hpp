#ifndef AGENT_H_GUARD
#define AGENT_H_GUARD

#include <vector>

#include "PRM.hpp"

namespace agent {
    enum class volume_type { CIRC, RECT };
};

class Agent {
public:
    glm::vec2 start, goal;
    Cspace_2D * cspace; //wish I could split this up into static/dynamic
    PRM * prm; //this seems wasteful...?

    glm::vec2 vel;//hack for ttc
    bool boid;//hack for boids

    std::vector<Node<glm::vec2> *> * plan;
    int completed_nodes;
    //"Model" -- nope -- too much work RN 
    agent::volume_type vt;
    BoundingVolume * bv;
    Agent(agent::volume_type vt, BoundingVolume * bv, glm::vec2 goal) {
        this->vt = vt;
        //todo: add error check for vt =~= bv
        this->bv = bv;
        this->start = bv->o;
        this->goal = goal;
        this->completed_nodes = 0;
        this->plan = nullptr;
        this->cspace = nullptr;
        this->prm = nullptr;

        this->boid = false;
        this->vel = glm::vec2(0);
    }
};


#endif//AGENT_H_GUARD