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
	void moveBy(int id, glm::vec3 t, double dt);
	void moveBy(std::vector<int> ids, glm::vec3 t, double dt);
    void simpleSimulate(double dt);
    //call this after instantiating the deformer. The vertices in the deformer are model coords.
    void convertMeshToWorldCoords();
    double timeElapsed = 0;
    virtual ~Slime();
};

#endif // SLIME_H_GUARD