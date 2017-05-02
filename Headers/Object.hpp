#ifndef OBJECT_H_GUARD
#define OBJECT_H_GUARD

#include <vector>
#include <algorithm> // min, max
#include "Structs.hpp"

#pragma warning(push, 0)
#include <glad/glad.h>
#include <glm/glm.hpp>
#pragma warning(pop)

class Object {
public:
	Object();
	Object(float x, float y);
	Object(glm::vec3 p);
	virtual ~Object();

	virtual void setParams(float x);
	virtual void setParams(float x, float y);
	virtual void setParams(float x, float y, float z);

	void setColor(float r, float g, float b);
	void setColor(glm::vec3 rgb);

	// Simple movement functions; we'll need to adapt these to however our objects move
	void moveBy(float x, float y, float z);
	void moveBy(glm::vec3 t);
	void moveTo(float x, float y, float z);
	void moveTo(glm::vec3 position);

	virtual void construct() = 0;

	void render(bufferContainer bc);

	// Member variables
    dynamics_comp dyn;

	glm::vec3 params;
	glm::vec3 color = glm::vec3(0, 0, 1);
	bool sphere;

	// Rendering variables
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> colors;
	std::vector<glm::vec3> normals;
	std::vector<GLuint> faces;
};

#endif // OBJECT_H_GUARD