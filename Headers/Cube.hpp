#ifndef	CUBE_H_GUARD
#define CUBE_H_GUARD

#include "Object.hpp"

class Cube : public Object {
public:
	Cube();
	Cube(float x, float y);
	Cube(float x, float y, float w, float h);
	Cube(float x, float y, float w, float h, float d);
	Cube(glm::vec3 p);
	Cube(glm::vec3 p, float w, float h);
	Cube(glm::vec3 p, float w, float h, float d);
	virtual ~Cube();

	void construct();

	// width is params[0]
	// height is params[1]
	// depth is params[2]
};

#endif // RECTANGLE_H_GUARD