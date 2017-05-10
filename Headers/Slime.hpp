#ifndef SLIME_H_GUARD
#define SLIME_H_GUARD
#include "Cube.hpp"
class Slime :
    public Cube
{
public:
    Slime();
    Slime(float h, float w, float d, float x, float y, float z);
    Slime(float h, float w, float d, glm::vec3 p);

    void moveBy(float x, float y, float z, double dt);
    void moveBy(glm::vec3 t, double dt);
    void moveTo(float x, float y, float z);
    void moveTo(glm::vec3 position);
    double aMin = .000000000001;
    double aMax = 10000000000;
    double b = 1;;

    virtual ~Slime();
};

#endif // OBJECT_H_GUARD