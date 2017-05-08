#include "Cube.hpp"

using glm::vec3;
using std::vector;
using std::min;
using std::max;

Cube::Cube() : Object() {
	params[0] = 1.0;
	params[1] = 1.0;
	params[2] = 1.0;
	construct();
}

Cube::Cube(float x, float y) : Object(vec3(x, y, 0.0)) {
	params[0] = 1.0;
	params[1] = 1.0;
	params[2] = 1.0;
	construct();
}

Cube::Cube(float x, float y, float w, float h) : Object(vec3(x, y, 0.0)) {
	params[0] = w;
	params[1] = h;
	params[2] = 1.0;
	construct();
}

Cube::Cube(float x, float y, float w, float h, float d) : Object(vec3(x, y, 0.0)) {
	params[0] = w;
	params[1] = h;
	params[2] = d;
	construct();
}

Cube::Cube(vec3 p) : Object(p) {
	params[0] = 1.0;
	params[1] = 1.0;
	params[2] = 1.0;
	construct();
}

Cube::Cube(vec3 p, float w, float h) : Object(p) {
	params[0] = w;
	params[1] = h;
	params[2] = 1.0;
	construct();
}

Cube::Cube(vec3 p, float w, float h, float d) : Object(p) {
	params[0] = w;
	params[1] = h;
	params[2] = d;
	construct();
}

Cube::~Cube() {}

void Cube::construct() {
    this->bv = new Rect(glm::vec2(dyn.pos[0], dyn.pos[1]), params[0], params[1]);

	vector<Vertex> vertices;
	for (int i = 0; i < 24; i++) {
		vertices.push_back(Vertex());
	}

	int m = 0;
	// Front face
	vertices[m*4 + 0].Position = dyn.pos + vec3(params[0]/2, -params[1]/2, dyn.pos[2] + params[2]);
	vertices[m*4 + 1].Position = dyn.pos + vec3(-params[0]/2, -params[1]/2, dyn.pos[2] + params[2]);
	vertices[m*4 + 2].Position = dyn.pos + vec3(params[0]/2, params[1]/2, dyn.pos[2] + params[2]);
	vertices[m*4 + 3].Position = dyn.pos + vec3(-params[0]/2, params[1]/2, dyn.pos[2] + params[2]);

	for (int i = 0; i < 4; i++)
		vertices[m*4 + i].Normal = vec3(0, 0, 1);

	m = 1;
	// Left face
	vertices[m*4 + 0].Position = dyn.pos + vec3(-params[0]/2, -params[1]/2, dyn.pos[2] + params[2]);
	vertices[m*4 + 1].Position = dyn.pos + vec3(-params[0]/2, -params[1]/2, dyn.pos[2] - params[2]);
	vertices[m*4 + 2].Position = dyn.pos + vec3(-params[0]/2, params[1]/2, dyn.pos[2] + params[2]);
	vertices[m*4 + 3].Position = dyn.pos + vec3(-params[0]/2, params[1]/2, dyn.pos[2] - params[2]);

	for (int i = 0; i < 4; i++)
		vertices[m*4 + i].Normal = vec3(-1, 0, 0);

	m = 2;
	// Back face
	vertices[m*4 + 0].Position = dyn.pos + vec3(-params[0]/2, -params[1]/2, dyn.pos[2] - params[2]);
	vertices[m*4 + 1].Position = dyn.pos + vec3(params[0]/2, -params[1]/2, dyn.pos[2] - params[2]);
	vertices[m*4 + 2].Position = dyn.pos + vec3(-params[0]/2, params[1]/2, dyn.pos[2] - params[2]);
	vertices[m*4 + 3].Position = dyn.pos + vec3(params[0]/2, params[1]/2, dyn.pos[2] - params[2]);

	for (int i = 0; i < 4; i++)
		vertices[m*4 + i].Normal = vec3(0, 0, -1);

	m = 3;
	// Right face
	vertices[m*4 + 0].Position = dyn.pos + vec3(params[0]/2, -params[1]/2, dyn.pos[2] - params[2]);
	vertices[m*4 + 1].Position = dyn.pos + vec3(params[0]/2, -params[1]/2, dyn.pos[2] + params[2]);
	vertices[m*4 + 2].Position = dyn.pos + vec3(params[0]/2, params[1]/2, dyn.pos[2] - params[2]);
	vertices[m*4 + 3].Position = dyn.pos + vec3(params[0]/2, params[1]/2, dyn.pos[2] + params[2]);

	for (int i = 0; i < 4; i++)
		vertices[m*4 + i].Normal = vec3(1, 0, 0);

	m = 4;
	// Top face
	vertices[m*4 + 0].Position = dyn.pos + vec3(params[0]/2, params[1]/2, dyn.pos[2] + params[2]);
	vertices[m*4 + 1].Position = dyn.pos + vec3(-params[0]/2, params[1]/2, dyn.pos[2] + params[2]);
	vertices[m*4 + 2].Position = dyn.pos + vec3(params[0]/2, params[1]/2, dyn.pos[2] - params[2]);
	vertices[m*4 + 3].Position = dyn.pos + vec3(-params[0]/2, params[1]/2, dyn.pos[2] - params[2]);

	for (int i = 0; i < 4; i++)
		vertices[m*4 + i].Normal = vec3(0, 1, 0);

	m = 5;
	// Top face
	vertices[m*4 + 0].Position = dyn.pos + vec3(params[0]/2, -params[1]/2, dyn.pos[2] - params[2]);
	vertices[m*4 + 1].Position = dyn.pos + vec3(-params[0]/2, -params[1]/2, dyn.pos[2] - params[2]);
	vertices[m*4 + 2].Position = dyn.pos + vec3(params[0]/2, -params[1]/2, dyn.pos[2] + params[2]);
	vertices[m*4 + 3].Position = dyn.pos + vec3(-params[0]/2, -params[1]/2, dyn.pos[2] + params[2]);

	for (int i = 0; i < 4; i++)
		vertices[m*4 + i].Normal = vec3(0, -1, 0);

	vector<GLuint> indices;
	for (int i = 0; i < 6; i++) {
		indices.push_back(i*4);
		indices.push_back(i*4 + 2);
		indices.push_back(i*4 + 3);

		indices.push_back(i*4);
		indices.push_back(i*4 + 3);
		indices.push_back(i*4 + 1);
	}

	mesh = new Mesh(vertices, indices);
}