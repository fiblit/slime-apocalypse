#include "Sphere.hpp"

using namespace glm;
using std::vector;
using std::min;
using std::max;

Sphere::Sphere() : Object() {
	params[0] = 1.0;
	params[1] = 1.0;
	params[2] = 1.0;
	construct();
}

Sphere::Sphere(float x, float y)  : Object(vec3(x, y, 0.0)) {
	params[0] = 1.0;
	params[1] = 1.0;
	params[2] = 1.0;
	construct();
}

Sphere::Sphere(float x, float y, float r) : Object(vec3(x, y, 0.0)) {
	params[0] = r;
	params[1] = r;
	params[2] = r;
	construct();
}

Sphere::Sphere(float x, float y, float z, float r) : Object(vec3(x, y, z)) {
	params[0] = r;
	params[1] = r;
	params[2] = r;
	construct();
}

Sphere::Sphere(float r) : Object() {
	params[0] = r;
	params[1] = r;
	params[2] = r;
	construct();
}

Sphere::Sphere(vec3 p, float r) : Object(p) {
	params[0] = r;
	params[1] = r;
	params[2] = r;
	construct();
}

Sphere::~Sphere() {}

void Sphere::construct() {
    this->bv = new Circ(glm::vec2(dyn.pos[0], dyn.pos[1]), params[0]);

	int stacks = 20;
	int slices = 20;

	vector<Vertex> vertices;
	vector<GLuint> indices;

	for (int i = 0; i < stacks+1; i++) {
		int nextStack = (i+1)%(stacks+1);
		double longitude = 2 * PI * i / stacks;
		//calculate the vertices and colors
		for (int j = 0; j < slices+1; j++) {
			Vertex vertex;

			int nextSlice = (j+1)%(slices+1);
			double colatitude = PI * j / slices;
			double x = dyn.pos[0] + params[0] * cos(longitude) * sin(colatitude);
			double y = dyn.pos[1] + params[0] * sin(longitude) * sin(colatitude);
			double z = dyn.pos[2] + params[0] * cos(colatitude);
			vertex.Position = vec3(x, y, z);
			vertex.Normal = vertex.Position - dyn.pos;
			vertices.push_back(vertex);

			indices.push_back(j + i*slices);
			indices.push_back(j + (nextStack)*slices);
			indices.push_back(nextSlice + (nextStack)*slices);

			indices.push_back(j + i*slices);
			indices.push_back(nextSlice + (nextStack)*slices);
			indices.push_back(nextSlice + i*slices);
		}
	}

	mesh = new Mesh(vertices, indices);
}