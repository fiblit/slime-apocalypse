#ifndef SPHERE_H_GUARD
#define SPHERE_H_GUARD

#include "Object.hpp"

#define PI 4.0*atan(1.0)

class Sphere : public Object {
public:
	Sphere();
	Sphere(float x, float y);
	Sphere(float x, float y, float r);
	Sphere(float r);
	Sphere(vec3 p, float r);
	virtual ~Sphere();

	void construct();

	Object *cObstacle(vec3 inputParams);

	bool collides(vec3 p);

	bool intersects(vec3 origin, vec3 ray);

	// params[0] is radius
};

#endif // SPHERE_H_GUARD