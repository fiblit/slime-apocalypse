#ifndef OBJECT_H_GUARD
#define OBJECT_H_GUARD

#include <vector>
#include <algorithm> // min, max
#include "Structs.hpp"

// Don't know how we bring these in from the top level
#include <glad/glad.h>// glad: an OpenGL function loader: https://github.com/Dav1dde/glad
#include <GLFW/glfw3.h>// glfw: a library for I/O and OpenGL context creation: http://www.glfw.org/
#include <glm/glm.hpp>// glm: OpenGL mathematics: http://glm.g-truc.net/0.9.8/index.html

using glm::vec3;
using std::vector;
using std::min;
using std::max;

class Object {
public:
	Object();
	Object(float x, float y);
	Object(vec3 p);
	virtual ~Object();

	virtual void setParams(float x);
	virtual void setParams(float x, float y);
	virtual void setParams(float x, float y, float z);

	void setColor(float r, float g, float b);
	void setColor(vec3 rgb);

	// Simple movement functions; we'll need to adapt these to however our objects move
	void moveBy(float x, float y, float z);
	void moveBy(vec3 t);
	void moveTo(float x, float y, float z);
	void moveTo(vec3 position);

	virtual Object *cObstacle(vec3 inputParams) = 0;

	virtual bool collides(vec3 p) = 0;

	virtual bool intersects(vec3 origin, vec3 ray) = 0;

	virtual void construct() = 0;

	void render(bufferContainer bc);

	// Member variables
	vec3 pos;
	vec3 params;
	vec3 color = vec3(0, 0, 1);
	bool sphere;

	// Rendering variables
	vector<vec3> vertices;
	vector<vec3> colors;
	vector<vec3> normals;
	vector<GLuint> faces;
};

#endif // OBJECT_H_GUARD