#ifndef LOCAL_MOTION_PLANNER_H_GUARD
#define LOCAL_MOTION_PLANNER_H_GUARD

//ttc or boids
/*

both are force-ish based

*/

#include "BoundingVolume.hpp"

namespace LMP {
    float ttc(BoundingVolume * i, glm::vec2 iv, BoundingVolume * j, glm::vec2 jv);
    float ttc_(Circ * i, glm::vec2 iv, Circ * j, glm::vec2 jv);
    float ttc_(Rect * i, glm::vec2 iv, Rect * j, glm::vec2 jv);
    float ttc_(Circ * i, glm::vec2 iv, Rect * j, glm::vec2 jv);
}

#endif//LOCAL_MOTION_PLANNER_H_GUARD