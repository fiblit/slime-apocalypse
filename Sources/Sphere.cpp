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
			double x = pos[0] + params[0] * cos(longitude) * sin(colatitude);
			double y = pos[1] + params[0] * sin(longitude) * sin(colatitude);
			double z = pos[2] + params[0] * cos(colatitude);
			vertices.push_back(vec3(x, y, z));
			colors.push_back(color);
			normals.push_back(pos - vertices.back());

			faces.push_back(j + i*slices);
			faces.push_back(j + (nextStack)*slices);
			faces.push_back(nextSlice + (nextStack)*slices);

			faces.push_back(j + i*slices);
			faces.push_back(nextSlice + (nextStack)*slices);
			faces.push_back(nextSlice + i*slices);
		}
	}
}

bool Sphere::collides(vec3 p) {
	return dot((p - dyn.pos), (p - dyn.pos)) < params[0]*params[0];
}

bool Sphere::intersects(vec3 origin, vec3 ray) {
	vec3 objectRay = origin - dyn.pos;
	vec3 pointRay = ray;
	pointRay = normalize(pointRay);
	float magnitude = ray.length();

	float discriminant = dot(pointRay, objectRay)*dot(pointRay, objectRay) - (dot(objectRay, objectRay) - params[0]*params[0]);

	if (discriminant < 0)
		return false;

	float distance1 = -dot(pointRay, objectRay) - sqrt(discriminant);
	float distance2 = -dot(pointRay, objectRay) + sqrt(discriminant);

	if (distance1 < magnitude && distance2 < magnitude && distance1 > 0 && distance2 > 0)
		return true;

	return false;
}