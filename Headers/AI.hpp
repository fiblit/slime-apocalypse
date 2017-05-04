#include "Object.hpp"
#include "PRM.hpp"
#include "BoundingVolume.hpp"
#include "LocalMotionPlanner.hpp"

namespace ai {
    extern Cspace2D * std_cspace;
    extern PRM * std_prm;
    //todo dalton: deprecate the following three in favor of spatial struct
    extern std::vector<Object *> std_NNai;
    extern std::vector<Object *> std_NNboids;
    extern std::vector<Object *> std_NNstatic;
    void init(std::vector<BoundingVolume *> bv, std::vector<Object *> NNai, std::vector<Object *> NNboids, std::vector<Object *> NNstatic);
    void animate_agents(std::vector<Object *> agents, GLfloat dt);
};
