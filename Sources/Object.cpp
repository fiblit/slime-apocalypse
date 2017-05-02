#include "Object.hpp"

using glm::vec3;
using std::vector;
using std::min;
using std::max;

Object::Object() {
	pos = vec3();
}

Object::Object(float x, float y) {
	pos = vec3(x, y, 0.0);
}

Object::Object(vec3 p) {
	pos = p;
}

Object::~Object() {}

void Object::setParams(float x) {
	params[0] = x;
	params[1] = x;
	params[2] = x;

	construct();
}

void Object::setParams(float x, float y) {
	params[0] = x;
	params[1] = y;
	params[2] = 0;

	construct();
}

void Object::setParams(float x, float y, float z) {
	params[0] = x;
	params[1] = y;
	params[2] = z;

	construct();
}

void Object::setColor(float r, float g, float b) {
	setColor(vec3(r, g, b));
}

void Object::setColor(vec3 rgb) {
	color = vec3(max(0.0f, min(1.0f, rgb[0])), max(0.0f, min(1.0f, rgb[1])), max(0.0f, min(1.0f, rgb[2])));
	for (int i = 0; i < colors.size(); i++) {
		colors[i] = color;
	}
}

// Simple movement functions; we'll need to adapt these to however our objects move
void Object::moveBy(float x, float y, float z) {
	moveBy(vec3(x, y, z));
}

// Simple movement functions; we'll need to adapt these to however our objects move
void Object::moveBy(vec3 t) {
	pos += t;
	for (int i = 0; i < vertices.size(); i++) {
		vertices[i] += t;
	}
}

// Simple movement functions; we'll need to adapt these to however our objects move
void Object::moveTo(float x, float y, float z) {
	moveTo(vec3(x, y, z));
}

// Simple movement functions; we'll need to adapt these to however our objects move
void Object::moveTo(vec3 position) {
	vec3 p = position - pos;
	pos += p;
	for (int i = 0; i < vertices.size(); i++) {
		vertices[i] += p;
	}
}

void Object::render(bufferContainer bc) {
	// I originally used a Globals namespace to access the vbos, but
	// we can just pass around a struct instead.
	// This really only works if the Scene class really does handle
	// all rendering.

	//using namespace Globals;

	glBindBuffer(GL_ARRAY_BUFFER, bc.position_vbo);
	if (!vertices.empty())
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0][0], GL_DYNAMIC_DRAW);
	else
		glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, bc.colors_vbo);
	if (!colors.empty())
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(colors[0]), &colors[0][0], GL_DYNAMIC_DRAW);
	else
		glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, bc.normals_vbo);
	if (!normals.empty())
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals[0]), &normals[0][0], GL_DYNAMIC_DRAW);
	else
		glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bc.faces_ibo);
	if (!faces.empty())
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(faces[0]), &faces[0], GL_STATIC_DRAW);
	else
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);

	glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, (GLvoid*) 0);
}