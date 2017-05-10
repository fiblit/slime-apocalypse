#include "AI.hpp"

Cspace2D * ai::std_cspace;
PRM * ai::std_prm;
BVH * ai::static_bvh;
BVH * ai::dynamic_bvh;

//todo: figure out how to handle varying size Cspace
void ai::init(std::vector<Object *> dynamics, std::vector<Object *> statics) {
    ai::static_bvh = new BVH(statics);
    ai::dynamic_bvh = new BVH(dynamics);
    /*
    for (Object * o : statics) {
        o->ai.method = ai_comp::Planner::NONE;
    }
    if (dynamics.size() > static_cast<size_t>(0)) {
        std::vector<BoundingVolume *> obs_bv;
        for (Object * s : statics)
            obs_bv.push_back(s->bv);
        ai::std_cspace = dynamics[0]->ai.cspace = new Cspace2D(obs_bv, dynamics[0]->bv);
        ai::std_prm = new PRM(/*NNai[0]->bv->o, NNai[0]->bv->o,*//*ai::std_cspace);

        for (Object * o : dynamics) {
            o->ai.method = ai_comp::Planner::INDY;
            o->ai.plan = new VecPoint();
            o->ai.num_done = 0;
            o->ai.cspace = ai::std_cspace;
            o->ai.prm = ai::std_prm;
        }
    }
    */
}

//move to AI/planner --- this is a force-based LMP
void ai::update_agents(std::vector<Object *> statics, std::vector<Object *> dynamics, Gtime::Timer * clock) {
    clock->pause();
    if (ai::static_bvh->size() != statics.size()) {
        delete ai::static_bvh;//probably very slow
        ai::static_bvh = new BVH(statics);
    }
    delete ai::dynamic_bvh;
    ai::dynamic_bvh = new BVH(dynamics);
    /*
    for (Object * d : dynamics) {
        if (d->ai.is_agent()) {
            if(d->ai.plan->size() == static_cast<size_t>(d->ai.num_done)/*GMP::invalid(d->ai.plan)*//*) {
                GMP::plan_one(d);
            }
            glm::vec2 f2d = LMP::calc_sum_force(d, ai::static_bvh, ai::dynamic_bvh, std::vector<Object *>());
            d->dyn.force += glm::vec3(f2d.x, 0, f2d.y);
        }
    }
    */
    clock->play();
}
