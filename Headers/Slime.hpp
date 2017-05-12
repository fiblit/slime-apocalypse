#ifndef SLIME_H_GUARD
#define SLIME_H_GUARD
#include "Sphere.hpp"
class Slime :
    public Sphere
{
public:
    Slime();
    Slime(float r, float x, float y, float z); 
    Slime(float r);
    Slime(float r, glm::vec3 p);

    void simulate(double dt) override;
    void moveBy(float x, float y, float z); 
    void moveBy(float x, float y, float z, double dt);
    void moveBy(glm::vec3 t);
    void moveBy(glm::vec3 t, double dt);
    void moveTo(float x, float y, float z);
    void moveTo(glm::vec3 position, double dt);
    void simpleSimulate(double dt);
    double timeElapsed = 0;
    virtual ~Slime();
};

#endif // SLIME_H_GUARD