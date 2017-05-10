#include "BVH.hpp"
#include "debug.hpp"

using namespace std;

BVH::BVH() {
    left = nullptr;
    right = nullptr;
}

BVH::BVH(vector<Object *> objects) {
    this->size_ = objects.size();

    //create two index arrays
    vector<Index> sorted_x = vector<Index>(objects.size());
    vector<Index> sorted_z = vector<Index>(objects.size());
    for (size_t i = 0; i < objects.size(); i++)
        //for sorted_x Index is (idx of obj for x, idx of zidx)
        //for sorted_z Index is (idx of obj for z, idx of xidx)
        //so .obj will always grab the object,
        //.oth the other table's index to the same object
        sorted_x[i] = sorted_z[i] = Index(i, 0);

    //sort by dimension
    std::sort(sorted_x.begin(), sorted_x.end(), [objects](Index a, Index b) {
        return objects[a.obj]->bv->o.x < objects[b.obj]->bv->o.x;
    });
    for (size_t i = 0; i < sorted_x.size(); i++)//connect index tables
        sorted_z[sorted_x[i].obj].oth = i;

    std::sort(sorted_z.begin(), sorted_z.end(), [objects](Index a, Index b) {
        return objects[a.obj]->bv->o.y < objects[b.obj]->bv->o.y;
    });
    for (size_t i = 0; i < sorted_z.size(); i++)//connect index tables
        sorted_x[sorted_z[i].oth].oth = i;

    construct_(objects, sorted_x, sorted_z);
}

void BVH::construct_(vector<Object *> objects, vector<Index> sorted_x, vector<Index> sorted_z) {
    D(assert(sorted_x.size() == sorted_z.size()));
    if (sorted_x.size() == static_cast<size_t>(1)) {
        //D(assert(sorted_x[0] == sorted_z[0]));
        this->right = nullptr;
        o = objects[sorted_x[0].obj];
        return;
    }

    //create bounding volume for this level, remembering the min/max in each dim
    float min_x, max_x, min_z, max_z;
    min_x = min_z = std::numeric_limits<float>::max();
    max_x = max_z = std::numeric_limits<float>::min();
    //tightly fit min_x/max_x/min_z/max_z
    for (size_t i = 0; i < sorted_x.size(); i++) {
        BoundingVolume * bv = objects[sorted_x[i].obj]->bv;
        float dim_x, dim_z;
        if (bv->vt == BoundingVolume::volume_type::RECT) {
            //add a nudge if you want fat BVs
            dim_x = static_cast<Rect *>(bv)->w;
            dim_z = static_cast<Rect *>(bv)->h;
        }
        else {
            dim_x = dim_z = static_cast<Circ *>(bv)->r;
        }

        float x_lo = bv->o.x - dim_x;
        float x_hi = bv->o.x + dim_x;
        float z_lo = bv->o.y - dim_z;
        float z_hi = bv->o.y + dim_z;
        if (x_lo < min_x) min_x = x_lo;
        if (x_hi > max_x) max_x = x_hi;
        if (z_lo < min_z) min_z = z_lo;
        if (z_hi > max_z) max_z = z_hi;
    }

    float dx = max_x - min_x;
    float dz = max_z - min_z;
    aabb = Rect(glm::vec2(min_x + dx/2, min_z + dz/2), dx, dz);

    //partition along longer axis, splitting indices equally
    vector<Index> x_rhs, x_lhs;
    vector<Index> z_rhs, z_lhs;
    if (dx > dz) {
        size_t half = static_cast<size_t>(sorted_x.size() / 2);
        size_t half_ceil = static_cast<size_t>((sorted_x.size() / 2) + 0.5);
        z_lhs = vector<Index>(half);
        x_lhs = vector<Index>(half);
        z_rhs = vector<Index>(half_ceil);
        x_rhs = vector<Index>(half_ceil);

        for (size_t i = 0; i < sorted_x.size(); i++) {
            if (i < half) {
                x_lhs[i] = sorted_x[i];
                z_lhs[sorted_x[i].oth] = sorted_z[sorted_x[i].oth];
            }
            else {
                x_rhs[i - half] = sorted_x[i];
                z_rhs[sorted_x[i].oth] = sorted_z[sorted_x[i].oth];
            }
        }

    }
    else {
        size_t half = static_cast<size_t>(sorted_z.size() / 2);
        size_t half_ceil = static_cast<size_t>((sorted_z.size() / 2) + 0.5);
        z_lhs = vector<Index>(half);
        x_lhs = vector<Index>(half);
        z_rhs = vector<Index>(half_ceil);
        x_rhs = vector<Index>(half_ceil);

        for (size_t i = 0; i < sorted_z.size(); i++) {
            if (i < half) {
                z_lhs.push_back(sorted_z[i]);
                x_lhs[sorted_z[i].oth] = sorted_x[sorted_z[i].oth];
            }
            else {
                z_rhs.push_back(sorted_z[i]);
                x_lhs[sorted_z[i].oth] = sorted_x[sorted_z[i].oth];
            }
        }
    }

    //probably the slowest part is the new, tbh XD
    left = new BVH();
    left->construct_(objects, x_lhs, z_lhs);
    right = new BVH();
    right->construct_(objects, x_rhs, z_rhs);
}

BVH::~BVH() {
    if (!is_leaf()) {
        delete left;
        delete right;
    }
}


vector<Object *> BVH::query(BoundingVolume * q) {
    vector<Object *> NN;
    query_(q, &NN);
    return NN;
}
void BVH::query_(BoundingVolume * q, vector<Object *> * NN) {
    //create cspace
    vector<BoundingVolume *> ms;
    if (is_leaf())
        ms = q->minkowskiSum(o->bv);
    else
        ms = q->minkowskiSum(&aabb);
    
    //test (in cspace) if q hits the BV
    for (BoundingVolume * bv : ms) {
        if (bv->is_collision(q->o)) {
            if (is_leaf())
                //leaves add their object (it's a near neighbor)
                NN->push_back(o);
            else {
                //otherwise recurse, as the top level was hit
                this->left->query_(q, NN);
                this->right->query_(q, NN);
            }  
        }
    }
}

//don't use, test later as the sqrt will probably make this slower
//this would be useful if it is faster, though, since circ/rect is common
//that would also mean I need to update my BV code.
bool BVH::circ_rect_collider_(Circ * c, Rect * r) {
    glm::vec2 L = r->o - c->o;
    L /= sqrt(glm::dot(L, L));
    L *= c->r;
    L += c->o;
    return r->is_collision(L);
}
