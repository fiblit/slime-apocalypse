#include "Sphere.hpp"

using namespace glm;
using std::vector;
using std::min;
using std::max;

Sphere::Sphere() : Object() {
	params[0] = 1.0;
	sphere = true;
	construct();
}

Sphere::Sphere(float x, float y)  : Object(vec3(x, y, 0.0)) {
	params[0] = 1.0;
	sphere = true;
	construct();
}

Sphere::Sphere(float x, float y, float r) : Object(vec3(x, y, 0.0)) {
	params[0] = r;
	sphere = true;
	construct();
}

Sphere::Sphere(float x, float y, float z, float r) : Object(vec3(x, y, z)) {
	params[0] = r;
	sphere = true;
	construct();
}

Sphere::Sphere(float r) : Object() {
	params[0] = r;
	sphere = true;
	construct();
}

Sphere::Sphere(vec3 p, float r) : Object(p) {
	params[0] = r;
	sphere = true;
	construct();
}

Sphere::~Sphere() {}



void Sphere::construct() {
    this->bv = new Circ(glm::vec2(dyn.pos[0], dyn.pos[1]), params[0]);

	vertices.clear();
	colors.clear();
	normals.clear();
	faces.clear();

	int stacks = 20;
	int slices = 20;
	for (int i = 0; i < stacks+1; i++) {
		int nextStack = (i+1)%(stacks+1);
		double longitude = 2 * PI * i / stacks;
		//calculate the vertices and colors
		for (int j = 0; j < slices+1; j++) {
			int nextSlice = (j+1)%(slices+1);
			double colatitude = PI * j / slices;
			double x = dyn.pos[0] + params[0] * cos(longitude) * sin(colatitude);
			double y = dyn.pos[1] + params[0] * sin(longitude) * sin(colatitude);
			double z = dyn.pos[2] + params[0] * cos(colatitude);
			vertices.push_back(vec3(x, y, z));
			colors.push_back(color);
			normals.push_back(dyn.pos - vertices.back());

			faces.push_back(j + i*slices);
			faces.push_back(j + (nextStack)*slices);
			faces.push_back(nextSlice + (nextStack)*slices);

			faces.push_back(j + i*slices);
			faces.push_back(nextSlice + (nextStack)*slices);
			faces.push_back(nextSlice + i*slices);
		}
	}
}
