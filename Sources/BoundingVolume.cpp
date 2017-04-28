#include "BoundingVolume.hpp"
Rect::Rect() {}
Rect::Rect(glm::vec2 o, float w, float h) {
    this->o = o; this->w = w; this->h = h;
}

Circ::Circ() {}
Circ::Circ(glm::vec2 o, float r) {
    this->o = o; this->r = r;
}

std::vector<BoundingVolume *> Circ::minkowskiSum(BoundingVolume * bv) {
    //I wish there was a way I didn't have to check the types..
    Circ* c = dynamic_cast<Circ*>(bv);
    if (c != nullptr) {
        return minkowskiSum_(c);
    }
    
    Rect * r = dynamic_cast<Rect*>(bv);
    if (r != nullptr) {
        return minkowskiSum_(r);
    }

    return std::vector<BoundingVolume *>();
}
std::vector<BoundingVolume *> Circ::minkowskiSum_(Circ * b) {
    std::vector<BoundingVolume *> bv = {
        new Circ(b->o, this->r + b->r) };
    return bv;
}
std::vector<BoundingVolume *> Circ::minkowskiSum_(Rect * b) {
    std::vector<BoundingVolume *> bv = {
        new Rect(b->o, b->w, 2 * this->r + b->h),
        new Rect(b->o, 2 * this->r + b->w, b->h),
        new Circ(glm::vec2(b->o.x + b->w / 2, b->o.y + b->h / 2), this->r),
        new Circ(glm::vec2(b->o.x - b->w / 2, b->o.y + b->h / 2), this->r),
        new Circ(glm::vec2(b->o.x + b->w / 2, b->o.y - b->h / 2), this->r),
        new Circ(glm::vec2(b->o.x - b->w / 2, b->o.y - b->h / 2), this->r),
    };
    return bv;
}

std::vector<BoundingVolume *> Rect::minkowskiSum(BoundingVolume * bv) {
    Circ* c = dynamic_cast<Circ*>(bv);
    if (c != nullptr) {
        return minkowskiSum_(c);
    }

    Rect * r = dynamic_cast<Rect*>(bv);
    if (r != nullptr) {
        return minkowskiSum_(r);
    }

    return std::vector<BoundingVolume *>();
}
std::vector<BoundingVolume *> Rect::minkowskiSum_(Rect * b) {
    std::vector<BoundingVolume *> bv = {
        new Rect(b->o, this->w + b->w, this->h + b->h) };
    return bv;
}
std::vector<BoundingVolume *> Rect::minkowskiSum_(Circ * b) {
    std::vector<BoundingVolume *> bv = {
        new Rect(b->o, this->w, 2 * b->r + this->h),
        new Rect(b->o, 2 *b->r + this->w, this->h),
        new Circ(glm::vec2(b->o.x + this->w / 2, b->o.y + this->h / 2), b->r),
        new Circ(glm::vec2(b->o.x - this->w / 2, b->o.y + this->h / 2), b->r),
        new Circ(glm::vec2(b->o.x + this->w / 2, b->o.y - this->h / 2), b->r),
        new Circ(glm::vec2(b->o.x - this->w / 2, b->o.y - this->h / 2), b->r),
    };
    return bv;
}


bool Circ::isCollision(glm::vec2 p) {
    glm::vec2 diff = p - this->o;
    return glm::dot(diff, diff) <= this->r * this->r;
}

bool Circ::lineOfSight(glm::vec2 a, glm::vec2 b, glm::vec2 Lab, float len2) {
    glm::vec2 Lbo = this->o - b;
    float r2 = this->r * this->r;
    if (glm::dot(Lbo, Lbo) <= r2) //point b inside circle
        return false; // HIT

    glm::vec2 Lao = this->o - a;
    //we don't use isCollision because we use a lot of these values again
    if (glm::dot(Lao, Lao) <= r2) //point a inside circle
        return false; // HIT

    float ang = glm::dot(Lab, Lao);
    glm::vec2 proj = Lab * (ang / len2);
    glm::vec2 rej = Lao - proj;
    float projlen2 = glm::dot(proj, proj);

    if (glm::dot(rej, rej) <= r2 //close enough tangentially
        && 0 <= ang              //point a before circle center
        && projlen2 <= len2)     //point b after circle center
        return false; // HIT

    return true; // else MISS
}

bool Rect::isCollision(glm::vec2 p) {
    return abs(p.x - this->o.x) <= this->w / 2
        && abs(p.y - this->o.y) <= this->h / 2;
}

bool Rect::lineOfSight(glm::vec2 a, glm::vec2 b, glm::vec2, float) {
    float left = this->o.x - this->w / 2;
    float right = this->o.x + this->w / 2;
    float top = this->o.y + this->h / 2;
    float bottom = this->o.y - this->h / 2;

    return !(axialLineSegLineSegCollision(a, b, left, 0, bottom, top)
        || axialLineSegLineSegCollision(a, b, right, 0, bottom, top)
        || axialLineSegLineSegCollision(a, b, bottom, 1, left, right)
        || axialLineSegLineSegCollision(a, b, top, 1, left, right));
}

//TODO: fix, so I can rotate rects
bool Rect::lineSegCollision(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4) {
    glm::vec2 pp[4] = { p1, p2, p3, p4 };
    glm::vec3 l[2], p[4], x;

    for (int i = 0; i < 4; i++)
        p[i] = glm::vec3(pp[i].x, pp[i].y, 1);

    l[0] = glm::cross(p[0], p[1]);
    l[1] = glm::cross(p[2], p[3]);
    x = glm::cross(l[0], l[1]);
    glm::vec2 px;
    px.x = x.x / x.z;
    px.y = x.y / x.z;

    float len2l1 = (l[0].y * l[0].y) + (l[0].x * l[0].x);
    float len2l2 = (l[1].y * l[1].y) + (l[1].x * l[1].x);

    glm::vec2 pp2x[4];
    for (int i = 0; i < 4; i++)
        pp2x[i] = px - pp[i];
    for (int i = 0; i < 2; i++)
        if (glm::dot(pp2x[i], pp2x[i]) > len2l1)
            return false;//miss
    for (int i = 3; i < 5; i++)
        if (glm::dot(pp2x[i], pp2x[i]) > len2l2)
            return false;//miss

    return true;//must have hit
}

bool Rect::axialLineSegLineSegCollision(glm::vec2 pp1, glm::vec2 pp2, float val, int axis, float oValLo, float oValHi) {
    glm::vec3 l = glm::cross(glm::vec3(pp1, 1), glm::vec3(pp2, 1));
    //ax+by+c = 0

    //vertical
    if (axis == 0) {// (1/val)*x + 0*y - 1 = 0 // x = val
        float yint = (-l[0] * val - l[2]) / l[1];
        return ((pp1.x <= val && val <= pp2.x) || (pp2.x <= val && val <= pp1.x))//val line hits lineseg
            && (oValLo <= yint && yint <= oValHi); //intersection on axial segment
    }
    //horizontal
    else {//if (axis == 1) {// 0x + (1/val)*y - 1 = 0 // y =val
        float xint = (-l[1] * val - l[2]) / l[0];
        return ((pp1.y <= val && val <= pp2.y) || (pp2.y <= val && val <= pp1.y))//axis line hits lineseg
            && (oValLo <= xint && xint <= oValHi); //intersection on axial segment
    }
}