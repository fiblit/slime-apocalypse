#ifndef SPHERE_H_GUARD
#define SPHERE_H_GUARD

#include "Object.hpp"

const GLdouble PI = 4.0*atan(1.0);

class Sphere : public Object {
public:
	Sphere();
	Sphere(float r, float x, float y, float z);
	Sphere(float r);
	Sphere(float r, glm::vec3 p);
	virtual ~Sphere();

    void simulate(double dt);
	void constructStandardMesh(bool override = false);
	void useStandardMesh();
	void useCustomMesh();

	static Mesh * standardMesh;
	bool usingStandardMesh = true;

	// may need to change from static const
	static const int stacks = 11;
	static const int slices = 11;

	// params[0] is radius
};

#endif // SPHERE_H_GUARD