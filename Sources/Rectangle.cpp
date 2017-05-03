#include "Rectangle.hpp"

using glm::vec3;
using std::vector;
using std::min;
using std::max;

Rectangle::Rectangle() : Object() {
	params[0] = 1.0;
	params[1] = 1.0;
	sphere = false;
	construct();
}

Rectangle::Rectangle(float x, float y) : Object(vec3(x, y, 0.0)) {
	params[0] = 1.0;
	params[1] = 1.0;
	sphere = false;
	construct();
}

Rectangle::Rectangle(float x, float y, float w, float h) : Object(vec3(x, y, 0.0)) {
	params[0] = w;
	params[1] = h;
	sphere = false;
	construct();
}

Rectangle::Rectangle(vec3 p) : Object(p) {
	params[0] = 1.0;
	params[1] = 1.0;
	sphere = false;
	construct();
}

Rectangle::Rectangle(vec3 p, float w, float h) : Object(p) {
	params[0] = w;
	params[1] = h;
	sphere = false;
	construct();
}

Rectangle::~Rectangle() {}

void Rectangle::construct() {
	vertices.clear();
	colors.clear();
	normals.clear();
	faces.clear();

	// Front face
	vertices.push_back(dyn.pos + vec3(params[0]/2, -params[1]/2, dyn.pos[2] + 1));
	vertices.push_back(dyn.pos + vec3(-params[0]/2, -params[1]/2, dyn.pos[2] + 1));
	vertices.push_back(dyn.pos + vec3(params[0]/2, params[1]/2, dyn.pos[2] + 1));
	vertices.push_back(dyn.pos + vec3(-params[0]/2, params[1]/2, dyn.pos[2] + 1));

	for (int i = 0; i < 4; i++)
		normals.push_back(vec3(0, 0, 1));

	// Left face
	vertices.push_back(dyn.pos + vec3(-params[0]/2, -params[1]/2, dyn.pos[2] + 1));
	vertices.push_back(dyn.pos + vec3(-params[0]/2, -params[1]/2, dyn.pos[2] - 1));
	vertices.push_back(dyn.pos + vec3(-params[0]/2, params[1]/2, dyn.pos[2] + 1));
	vertices.push_back(dyn.pos + vec3(-params[0]/2, params[1]/2, dyn.pos[2] - 1));

	for (int i = 0; i < 4; i++)
		normals.push_back(vec3(-1, 0, 0));

	// Back face
	vertices.push_back(dyn.pos + vec3(-params[0]/2, -params[1]/2, dyn.pos[2] - 1));
	vertices.push_back(dyn.pos + vec3(params[0]/2, -params[1]/2, dyn.pos[2] - 1));
	vertices.push_back(dyn.pos + vec3(-params[0]/2, params[1]/2, dyn.pos[2] - 1));
	vertices.push_back(dyn.pos + vec3(params[0]/2, params[1]/2, dyn.pos[2] - 1));

	for (int i = 0; i < 4; i++)
		normals.push_back(vec3(0, 0, -1));

	// Right face
	vertices.push_back(dyn.pos + vec3(params[0]/2, -params[1]/2, dyn.pos[2] - 1));
	vertices.push_back(dyn.pos + vec3(params[0]/2, -params[1]/2, dyn.pos[2] + 1));
	vertices.push_back(dyn.pos + vec3(params[0]/2, params[1]/2, dyn.pos[2] - 1));
	vertices.push_back(dyn.pos + vec3(params[0]/2, params[1]/2, dyn.pos[2] + 1));

	for (int i = 0; i < 4; i++)
		normals.push_back(vec3(1, 0, 0));

	// Top face
	vertices.push_back(dyn.pos + vec3(params[0]/2, params[1]/2, dyn.pos[2] + 1));
	vertices.push_back(dyn.pos + vec3(-params[0]/2, params[1]/2, dyn.pos[2] + 1));
	vertices.push_back(dyn.pos + vec3(params[0]/2, params[1]/2, dyn.pos[2] - 1));
	vertices.push_back(dyn.pos + vec3(-params[0]/2, params[1]/2, dyn.pos[2] - 1));

	for (int i = 0; i < 4; i++)
		normals.push_back(vec3(0, 1, 0));

	// Bottom face
	vertices.push_back(dyn.pos + vec3(params[0]/2, -params[1]/2, dyn.pos[2] - 1));
	vertices.push_back(dyn.pos + vec3(-params[0]/2, -params[1]/2, dyn.pos[2] - 1));
	vertices.push_back(dyn.pos + vec3(params[0]/2, -params[1]/2, dyn.pos[2] + 1));
	vertices.push_back(dyn.pos + vec3(-params[0]/2, -params[1]/2, dyn.pos[2] + 1));

	for (int i = 0; i < 4; i++)
		normals.push_back(vec3(0, -1, 0));


	for (int i = 0; i < 24; i++)
		colors.push_back(color);

	for (int i = 0; i < 6; i++) {
		faces.push_back(i*4);
		faces.push_back(i*4 + 2);
		faces.push_back(i*4 + 3);

		faces.push_back(i*4);
		faces.push_back(i*4 + 3);
		faces.push_back(i*4 + 1);
	}
}