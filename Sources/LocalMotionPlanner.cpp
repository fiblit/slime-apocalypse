#include "LocalMotionPlanner.hpp"
float LMP::ttc(BoundingVolume * i, glm::vec2 iv, BoundingVolume * j, glm::vec2 jv) {
    //I wish there was a way I didn't have to check the types..
    /*Circ* c = dynamic_cast<Circ*>(i);
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
                return ttc_(r, iv, r2, jv);

            Circ * c = dynamic_cast<Circ *>(j);
            if (c != nullptr)
                return ttc_(c, iv, r, jv);
        }
    }
    */

    return LMP::ttc_(static_cast<Circ *>(i), iv, static_cast<Circ *>(j), jv);
}

float LMP::ttc_(Circ * i, glm::vec2 iv, Circ * j, glm::vec2 jv) {
    float r = i->r + j->r;
    glm::vec2 w = j->o - i->o;
    float c = glm::dot(w, w) - r * r;
    if (c < 0) // agents are colliding
        return 0;

    glm::vec2 v = iv - jv;
    float a = glm::dot(v, v);
    float b = glm::dot(w, v);
    float d = b*b - a*c;
    if (d <= 0)
        return std::numeric_limits<float>::max();

    float tau = (b - sqrt(d)) / a;
    if (tau < 0)
        return std::numeric_limits<float>::max();
    return tau;
}

//tbd
float LMP::ttc_(Rect * i, glm::vec2 iv, Rect * j, glm::vec2 jv) {
    glm::vec2 o = j->o - i->o;
    glm::vec2 v = iv - jv;
    float w = i->w / 2 + j->w / 2;
    float h = i->h / 2 + j->h / 2;

    //[top; bot] ~ tau
    float top = (w - o.x) / v.x;
    float bot = (h - o.y) / v.y;

    //if ()
    return 0;
}

//tbd
float LMP::ttc_(Circ * i, glm::vec2 iv, Rect * j, glm::vec2 jv) {
    glm::vec2 o = j->o - i->o;
    glm::vec2 v = iv - jv;
    float w = i->r + j->w / 2;
    float h = i->r + j->h / 2;

    //[top; bot] ~ tau
    float top = (w - o.x) / v.x;
    float bot = (h - o.y) / v.y;

    //if ()
    return 0;
}
