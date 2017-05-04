#include "AI.hpp"

Cspace2D * ai::std_cspace;
PRM * ai::std_prm;
std::vector<Object *> ai::std_NNai;
std::vector<Object *> ai::std_NNboids;
std::vector<Object *> ai::std_NNstatic;

//move AI/planner
//refactor
//todo dalton: replace NN's with every object/the tree struct access
void ai::init(std::vector<BoundingVolume *> bv, std::vector<Object *> NNai, std::vector<Object *> NNboids, std::vector<Object *> NNstatic) {
    //try to only use circ agents(&statics) since I did not expand ttc for rects yet
    ai::std_cspace = new Cspace2D(bv, NNai[0]->bv);//todo dalton: null value checks
    ai::std_prm = new PRM(/*NNai[0]->bv->o, NNai[0]->bv->o,*/ ai::std_cspace);
    ai::std_NNai = NNai;
    ai::std_NNboids = NNboids;
    ai::std_NNstatic = NNstatic;
}

//move to AI/planner --- this is a force-based LMP
void ai::animate_agents(std::vector<Object *> agents, GLfloat dt) {
    for (Object * a : agents)
        LMP::calc_force(a, ai::std_NNai, ai::std_NNboids, ai::std_NNstatic, ai::std_cspace, dt);

    for (Object * a : agents)
        a->moveTo(glm::vec3(a->bv->o.x, 0.f, a->bv->o.y));
}
