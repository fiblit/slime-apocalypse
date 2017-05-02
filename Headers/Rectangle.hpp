#ifndef	RECTANGLE_H_GUARD
#define RECTANGLE_H_GUARD

#include "Object.hpp"

class Rectangle : public Object {
public:
	Rectangle();
	Rectangle(float x, float y);
	Rectangle(float x, float y, float w, float h);
	Rectangle(vec3 p);
	Rectangle(vec3 p, float w, float h);
	virtual ~Rectangle();

	void construct();

	Object *cObstacle(vec3 inputParams);

    //replace by my BoundingVolume
	bool collides(vec3 p);
	bool intersects(vec3 origin, vec3 ray);

	// width is params[0]
	// height is params[1]
};

#endif // RECTANGLE_H_GUARD