#ifndef GLOBAL_MOTION_PLANNER_H_GUARD
#define GLOBAL_MOTION_PLANNER_H_GUARD

#include "PRM.hpp"
#include "Object.hpp"
#include "Timer.hpp"
#include <vector>

namespace GMP {
    VecPoint * find_path_UCS(Graph<glm::vec2> * roadmap);
    VecPoint * find_path_Astar(float e, Graph<glm::vec2> * roadmap);
    void replan(std::vector<Object *> agents);
    void plan_one(Object * agent);
};

#endif//GLOBAL_MOTION_PLANNER_H_GUARD