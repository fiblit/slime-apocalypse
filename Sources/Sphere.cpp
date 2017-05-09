#include "Sphere.hpp"

using namespace glm;
using std::vector;
using std::min;
using std::max;

Mesh * Sphere::standardMesh;

Sphere::Sphere() : Object() {
	params[0] = 1.0;
	params[1] = 1.0;
	params[2] = 1.0;
	constructStandardMesh();
}

Sphere::Sphere(float r, float x, float y, float z) : Object(vec3(x, y, z)) {
	params[0] = r;
	params[1] = r;
	params[2] = r;
	constructStandardMesh();
}

Sphere::Sphere(float r) : Object() {
	params[0] = r;
	params[1] = r;
	params[2] = r;
	constructStandardMesh();
}

Sphere::Sphere(float r, vec3 p) : Object(p) {
	params[0] = r;
	params[1] = r;
	params[2] = r;
	constructStandardMesh();
}

Sphere::~Sphere() {
	if (mesh && mesh != standardMesh)
		delete mesh;
}

void Sphere::useStandardMesh() {
	if (usingStandardMesh)
		return;

	if (mesh)
		delete mesh;

	usingStandardMesh = true;
	mesh = standardMesh;
}

void Sphere::useCustomMesh() {
	if (!usingStandardMesh)
		return; // might want to make it re-copy the standard in this case?

	usingStandardMesh = false;
	mesh = standardMesh->copy();
}

void Sphere::constructStandardMesh(bool override) {
    this->bv = new Circ(glm::vec2(dyn.pos[0], dyn.pos[1]), params[0]);

	if (!override && standardMesh != NULL) {
		mesh = standardMesh;
		return;
	}

	int stacks = 20; // TODO: need a way to make these dynamic
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
			double x = cos(longitude) * sin(colatitude);
			double y = sin(longitude) * sin(colatitude);
			double z = cos(colatitude);
			vertex.Position = vec3(x, y, z);
			vertex.Normal = glm::normalize(vertex.Position);
			vertices.push_back(vertex);

			indices.push_back(j + i*slices);
			indices.push_back(j + (nextStack)*slices);
			indices.push_back(nextSlice + (nextStack)*slices);

			indices.push_back(j + i*slices);
			indices.push_back(nextSlice + (nextStack)*slices);
			indices.push_back(nextSlice + i*slices);
		}
	}
	if (override) {
		standardMesh->vertices = vertices;
		standardMesh->indices = indices;
	}
	else {
		standardMesh = new Mesh(vertices, indices);
		mesh = standardMesh;
	}
}
