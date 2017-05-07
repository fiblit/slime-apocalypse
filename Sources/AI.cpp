#include "AI.hpp"

Cspace2D * ai::std_cspace;
PRM * ai::std_prm;
std::vector<Object *> ai::std_NNai;
std::vector<Object *> ai::std_NNboids;
std::vector<Object *> ai::std_NNstatic;

//todo dalton: replace NN's with every object/the tree struct access
void ai::init(std::vector<Object *> NNai, std::vector<Object *> NNboids, std::vector<Object *> NNstatic) {
    //try to only use circ agents(&statics) since I did not expand ttc for rects yet
    if (NNai.size() > static_cast<size_t>(0)) {
        std::vector<BoundingVolume *> obs_bv;
        for (Object * s : NNstatic)
            obs_bv.push_back(s->bv);
        ai::std_cspace = NNai[0]->ai.cspace = new Cspace2D(obs_bv, NNai[0]->bv);
        ai::std_prm = new PRM(/*NNai[0]->bv->o, NNai[0]->bv->o,*/ai::std_cspace);
        ai::std_NNai = NNai;
        ai::std_NNboids = NNboids;
        ai::std_NNstatic = NNstatic;
    }
}

//move to AI/planner --- this is a force-based LMP
void ai::update_agents(std::vector<Object *> agents) {
    for (Object * a : agents) {
        glm::vec2 f2d = LMP::calc_sum_force(a, ai::std_NNai, ai::std_NNboids, ai::std_NNstatic);
        a->dyn.force += glm::vec3(f2d.x, 0, f2d.y);
    }
}
