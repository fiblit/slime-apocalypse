#include "AI.hpp"

Cspace2D * ai::std_cspace;
PRM * ai::std_prm;
std::vector<Object *> ai::std_NNai;
std::vector<Object *> ai::std_NNboids;
std::vector<Object *> ai::std_NNstatic;

//todo dalton: replace NN's with every object/the tree struct access
void ai::init(std::vector<BoundingVolume *> bv, std::vector<Object *> NNai, std::vector<Object *> NNboids, std::vector<Object *> NNstatic) {
    //try to only use circ agents(&statics) since I did not expand ttc for rects yet
    if (NNai.size() > static_cast<size_t>(0)) {
        ai::std_cspace = new Cspace2D(bv, NNai[0]->bv);
        ai::std_prm = new PRM(/*NNai[0]->bv->o, NNai[0]->bv->o,*/ ai::std_cspace);
        ai::std_NNai = NNai;
        ai::std_NNboids = NNboids;
        ai::std_NNstatic = NNstatic;
    }
}

//move to AI/planner --- this is a force-based LMP
//todo dalton: decouple the physics integration
void ai::update_agents(std::vector<Object *> agents, GLfloat dt) {
    for (Object * a : agents)
        a->dyn.force += 
            LMP::calc_sum_force(
                a,
                ai::std_NNai, ai::std_NNboids, ai::std_NNstatic, 
                ai::std_cspace, dt);

   // for (Object * a : agents)
   //     a->moveTo(glm::vec3(a->bv->o.x, 0.f, a->bv->o.y));
}
