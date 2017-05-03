#ifndef AGENT_H_GUARD
#define AGENT_H_GUARD

#include <vector>

#include "PRM.hpp"

class Agent {
public:
    glm::vec2 start, goal;

    //deprecate in favor of the grouping strategy
    Cspace2D * cspace; //wish I could split this up into static/dynamic
    PRM * prm; //this seems wasteful...?

    bool boid;//hack for boids

    //somehow this needs to be abstracted away from Object
    //Perhaps "Agent" can be a component tacked onto Objects
    //In turn, BoundingVolumes would either be tacked on the Agents or the Objects
    std::vector<Node<glm::vec2> *> * plan;
    int completed_nodes;
    //"Model" -- nope -- too much work RN 
    BoundingVolume * bv;
    Agent(BoundingVolume * bv, glm::vec2 goal) {
        //todo: add error check for vt =~= bv
        this->bv = bv;
        this->start = bv->o;
        this->goal = goal;
        this->completed_nodes = 0;
        this->plan = nullptr;
        this->cspace = nullptr;
        this->prm = nullptr;

        this->boid = false;
    }
};


#endif//AGENT_H_GUARD