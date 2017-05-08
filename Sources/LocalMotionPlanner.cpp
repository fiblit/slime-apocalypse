#include "LocalMotionPlanner.hpp"
#include "debug.hpp"
#include <limits>

//TODO: properly polymorph for all BoundingVolumes
float LMP::ttc(BoundingVolume * i, glm::vec2 iv, BoundingVolume * j, glm::vec2 jv) {
    //I wish there was a way I didn't have to check the types..
    Circ* c = dynamic_cast<Circ*>(i);
    if (c != nullptr) {
        Circ * c2 = dynamic_cast<Circ *>(j);
        if (c2 != nullptr)
            return ttc_(c, iv, c2, jv);

        Rect * r2 = dynamic_cast<Rect *>(j);
        if (r2 != nullptr)
            return ttc_(c, iv, r2, jv);
    }
    else {
        Rect * r = dynamic_cast<Rect*>(i);
        if (r != nullptr) {
            Rect * r2 = dynamic_cast<Rect *>(j);
            if (r2 != nullptr)
                return LMP::ttc_(r, iv, r2, jv);

            Circ * c = dynamic_cast<Circ *>(j);
            if (c != nullptr)
                return LMP::ttc_(c, iv, r, jv);
        }
    }
    return std::numeric_limits<float>::max();
}

float LMP::ttc_(Circ * i, glm::vec2 iv, Circ * j, glm::vec2 jv) {
    float r = i->r + j->r;
    glm::vec2 w = j->o - i->o;
    float w2 = glm::dot(w, w);
    float c = w2 - r * r;
    if (c < 0) {// agents are colliding
        //return 0; //original

        //as per Stephen Guy's suggestion; halve the radii when colliding
        //as per Caleb Biasco's suggestion; use the smallest radii, not both
        float smaller = (i->r > j->r ? j->r : i->r);
        r -= smaller;
        c = w2 - r * r;
    }

    glm::vec2 v = iv - jv;
    float a = glm::dot(v, v);
    float b = - glm::dot(w, v);
    float d = b*b - a*c;
    if (d <= 0)//improper solution
        return std::numeric_limits<float>::max();

    float tau = (b - sqrt(d)) / a;
    if (tau < 0)//ttc is in the past
        return std::numeric_limits<float>::max();

    return tau;
}

//finds the ttc via a component analysis of the velocity vectors
float LMP::ttc_(Rect * i, glm::vec2 iv, Rect * j, glm::vec2 jv) {
    float dim_x = i->w + j->w;
    float dim_y = i->h + j->h;
    
    float dv_x = jv.x - iv.x;
    float dv_y = jv.y - iv.y;
    
    float do_x = j->o.x - i->o.x;
    float do_y = j->o.y - i->o.y;
    
    float t_x_hi = (dim_x - do_x) / dv_x;
    float t_y_hi = (dim_y - do_y) / dv_y;

    float t_x_lo = (-dim_x - do_x) / dv_x;
    float t_y_lo = (-dim_y - do_y) / dv_y;

    if (t_x_hi < t_y_lo || t_x_lo > t_y_hi)//no collision past or present
        return std::numeric_limits<float>::max();
    else {
        float t_lo = (t_x_lo > t_y_lo ? t_x_lo : t_y_lo);
        float t_hi = (t_x_hi > t_y_hi ? t_x_hi : t_y_hi);
        if (t_hi < 0)//ttc in the past
            return std::numeric_limits<float>::max();
        else if (t_lo < 0)
            return 0; //collision is current
        else
            return t_lo;
    }
}

//TODO dalton
float LMP::ttc_(Circ * /*i*/, glm::vec2 /*iv*/, Rect * /*j*/, glm::vec2 /*jv*/) {
    //glm::vec2 o = j->o - i->o;
    //glm::vec2 v = iv - jv;
    //float w = i->r + j->w / 2;
    //float h = i->r + j->h / 2;

    //[top; bot] ~ tau
    //float top = (w - o.x) / v.x;
    //float bot = (h - o.y) / v.y;

    //if ()
    return 0;
}

glm::vec2 LMP::lookahead(Object * a, glm::vec2 target) {
    glm::vec2 t_new = target;
    if (static_cast<size_t>(a->ai.num_done) < a->ai.plan->size()) {
        t_new = (*(a->ai.plan))[a->ai.num_done]->data;

        while (static_cast<size_t>(a->ai.num_done) + 1 < a->ai.plan->size()
            && a->ai.cspace->line_of_sight(a->bv->o, (*(a->ai.plan))[a->ai.num_done + 1]->data)) {
            a->ai.num_done++;
            t_new = (*(a->ai.plan))[a->ai.num_done]->data;
        }
    }
    else
        t_new = (*(a->ai.plan))[a->ai.plan->size() - 1]->data;
    return t_new;
}

glm::vec2 LMP::ttc_forces_(double ttc, glm::vec2 dir) {
    dir /= glm::length(dir);

    double t_h = 5.0;//seconds
    double mag = 0;
    if (ttc >= 0 && ttc <= t_h)
        mag = (t_h - ttc) / (ttc + 0.001);
    mag = mag > 20 ? 20 : mag;
    return glm::vec2(mag * dir.x, mag * dir.y);
}
glm::vec2 LMP::ttc_forces(Object * a, Circ * b, float ttc) {
    glm::vec2 V_dt(a->dyn.vel);
    glm::vec2 dir = (a->bv->o + V_dt - b->o);
    return ttc_forces_(ttc, dir);
}
glm::vec2 LMP::ttc_forces(Object * a, Object * b, float ttc) {
    glm::vec2 V_dt(a->dyn.vel * ttc);
    glm::vec2 bV_dt(b->dyn.vel * ttc);
    glm::vec2 dir = (a->bv->o + V_dt - b->bv->o - bV_dt);
    return ttc_forces_(ttc, dir);
}

//distance to leader, weak close, strong far
//TOOD: replace near_pack with the ability to access/search a spatial struct
glm::vec2 follow_force(Object * lead, std::vector<Object *> near_pack) {
    GLfloat ff0_r = 2.0f;//radius of following force of 0
    GLfloat ff1_r = 10.0f;//radius of following force of 1 towards leader

    glm::vec2 ff;
    for (Object * b : near_pack) {
        //todo dalton: del o ref
        glm::vec2 toLeader = lead->bv->o - b->bv->o;
        float dist2_to_leader = glm::dot(toLeader, toLeader);
        if (dist2_to_leader < ff0_r * ff0_r)
            ff = glm::vec2(0);
        else
            ff = toLeader * (dist2_to_leader - ff0_r) / (ff1_r - ff0_r);
    }
    return ff;
}

//TOOD: replace near_boids with the ability to access/search a spatial struct of boids
glm::vec2 boid_force(Object * a, std::vector<Object *> near_boids) {
    const float boid_speed = 1.2f;

    glm::vec2 avg_vel(0, 0), avg_pos(0, 0), avg_dir(0, 0);
    GLfloat cohes_r_look = 1.0f, align_r_look = 1.0f, separ_r_look = .5f;//limit to search for forces for boidlings
    glm::vec2 align_force, cohesion_force, follow_force, spread_force;

    for (size_t i = 0; i < near_boids.size(); i++) {
        Object * boid = near_boids[i];
        if (boid == a)
            continue;
        glm::vec2 dist = boid->bv->o - a->bv->o;
        float fi = static_cast<float>(i);
        if (glm::dot(dist, dist) < align_r_look * align_r_look)
            avg_vel = (avg_vel * fi + glm::vec2(boid->dyn.vel.x, boid->dyn.vel.z)) / (fi + 1.f);
        if (glm::dot(dist, dist) < cohes_r_look * cohes_r_look)
            avg_pos = (avg_pos * fi + glm::vec2(boid->bv->o)) / (fi + 1.f);
    }

    /* alignnment force */
    //average velocity; pull towards that
    float norm = glm::length(avg_vel);
    if (norm != 0)
        avg_vel /= norm;
    align_force = (avg_vel - glm::vec2(a->dyn.vel.x, a->dyn.vel.z)) * boid_speed;

    /* cohesion force */
    //average cohesion; pull towards that
    cohesion_force = avg_pos - a->bv->o;

    /* separation force */
    //force from inverted direction of nearest neighbours
    for (size_t i = 0; i < near_boids.size(); i++) {
        Object * boid = near_boids[i];
        if (boid == a)
            continue;
        glm::vec2 toBoid = boid->bv->o - a->bv->o;
        float dist2_to_boid = glm::dot(toBoid, toBoid);

        if (dist2_to_boid < separ_r_look * separ_r_look)
            spread_force += -toBoid / (10 * sqrt(dist2_to_boid));
    }

    glm::vec2 boid_force = align_force + cohesion_force + spread_force;
    if (glm::dot(boid_force, boid_force) > 20 * 20) {
        boid_force /= glm::length(boid_force);
        boid_force *= 20.f;
    }
    return boid_force;
}

//TODO: replace NN with spatial search callback
glm::vec2 LMP::calc_sum_force(Object * a, std::vector<Object *> NNai, std::vector<Object *> NNboids, std::vector<Object *> NNstatic) {
    float speed = 1.0f; // x m/s
    glm::vec2 goal_vel;

    //if there is a plan, follow it
    if (a->ai.has_plan()) {
        a->ai.goal = LMP::lookahead(a, a->ai.goal);
        goal_vel = (a->ai.goal - a->bv->o) / glm::distance(a->ai.goal, a->bv->o) * (speed /* * dt */);
    }
    else {
        a->ai.goal = a->bv->o;
        goal_vel = glm::vec2(0);
    }

    /* ttc - approximate */
    glm::vec2 goal_F;
    //using ai_comp::Planner;
    if (a->ai.method == ai_comp::Planner::BOID || a->ai.method == ai_comp::Planner::PACK)
        goal_F = glm::vec2(0);
    else
        goal_F = 2.0f*(goal_vel - glm::vec2(a->dyn.vel.x, a->dyn.vel.z));

    glm::vec2 ttc_F(0), boid_F(0);
    for (Object * b : NNai) {
        if (a == b)
            continue;
        double ttc = LMP::ttc(a->bv, glm::vec2(a->dyn.vel.x, a->dyn.vel.z), b->bv, glm::vec2(b->dyn.vel.x, b->dyn.vel.z));
        if (ttc > 5/*seconds*/)
            continue;
        ttc_F += LMP::ttc_forces(a, b, static_cast<float>(ttc));
    }

    for (Object * s : NNstatic) {
        //TH
        Circ * c = dynamic_cast<Circ *>(s->bv);

        double ttc = LMP::ttc(a->bv, glm::vec2(a->dyn.vel.x, a->dyn.vel.z), c, glm::vec2(0));

        if (ttc > 5)
            continue;

        ttc_F += ttc_forces(a, c, static_cast<float>(ttc));
    }

    if (a->ai.has_boid_f())
        boid_F += boid_force(a, NNboids);

    return goal_F + ttc_F + boid_F;
}
