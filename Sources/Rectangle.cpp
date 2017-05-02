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
	vertices.push_back(pos + vec3(params[0]/2, -params[1]/2, pos[2] + 1));
	vertices.push_back(pos + vec3(-params[0]/2, -params[1]/2, pos[2] + 1));
	vertices.push_back(pos + vec3(params[0]/2, params[1]/2, pos[2] + 1));
	vertices.push_back(pos + vec3(-params[0]/2, params[1]/2, pos[2] + 1));

	for (int i = 0; i < 4; i++)
		normals.push_back(vec3(0, 0, 1));

	// Left face
	vertices.push_back(pos + vec3(-params[0]/2, -params[1]/2, pos[2] + 1));
	vertices.push_back(pos + vec3(-params[0]/2, -params[1]/2, pos[2] - 1));
	vertices.push_back(pos + vec3(-params[0]/2, params[1]/2, pos[2] + 1));
	vertices.push_back(pos + vec3(-params[0]/2, params[1]/2, pos[2] - 1));

	for (int i = 0; i < 4; i++)
		normals.push_back(vec3(-1, 0, 0));

	// Back face
	vertices.push_back(pos + vec3(-params[0]/2, -params[1]/2, pos[2] - 1));
	vertices.push_back(pos + vec3(params[0]/2, -params[1]/2, pos[2] - 1));
	vertices.push_back(pos + vec3(-params[0]/2, params[1]/2, pos[2] - 1));
	vertices.push_back(pos + vec3(params[0]/2, params[1]/2, pos[2] - 1));

	for (int i = 0; i < 4; i++)
		normals.push_back(vec3(0, 0, -1));

	// Right face
	vertices.push_back(pos + vec3(params[0]/2, -params[1]/2, pos[2] - 1));
	vertices.push_back(pos + vec3(params[0]/2, -params[1]/2, pos[2] + 1));
	vertices.push_back(pos + vec3(params[0]/2, params[1]/2, pos[2] - 1));
	vertices.push_back(pos + vec3(params[0]/2, params[1]/2, pos[2] + 1));

	for (int i = 0; i < 4; i++)
		normals.push_back(vec3(1, 0, 0));

	// Top face
	vertices.push_back(pos + vec3(params[0]/2, params[1]/2, pos[2] + 1));
	vertices.push_back(pos + vec3(-params[0]/2, params[1]/2, pos[2] + 1));
	vertices.push_back(pos + vec3(params[0]/2, params[1]/2, pos[2] - 1));
	vertices.push_back(pos + vec3(-params[0]/2, params[1]/2, pos[2] - 1));

	for (int i = 0; i < 4; i++)
		normals.push_back(vec3(0, 1, 0));

	// Bottom face
	vertices.push_back(pos + vec3(params[0]/2, -params[1]/2, pos[2] - 1));
	vertices.push_back(pos + vec3(-params[0]/2, -params[1]/2, pos[2] - 1));
	vertices.push_back(pos + vec3(params[0]/2, -params[1]/2, pos[2] + 1));
	vertices.push_back(pos + vec3(-params[0]/2, -params[1]/2, pos[2] + 1));

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

bool Rectangle::collides(vec3 p) {
	return p[0] > pos[0]-params[0]/2 && p[0] < pos[0]+params[0]/2 &&
		p[1] > pos[1]-params[1]/2 && p[1] < pos[1]+params[1]/2;
}

// Code adapted from Alejo's rectangle intersection solution at http://stackoverflow.com/a/293052
bool Rectangle::intersects(vec3 origin, vec3 ray) {
	vec3 topLeft, topRight, bottomLeft, bottomRight, start, end;
	topLeft = vec3(pos[0] - params[0]/2, pos[1] + params[1]/2, 0);
	topRight = vec3(pos[0] + params[0]/2, pos[1] + params[1]/2, 0);
	bottomLeft = vec3(pos[0] - params[0]/2, pos[1] - params[1]/2, 0);
	bottomRight = vec3(pos[0] + params[0]/2, pos[1] - params[1]/2, 0);

	start = origin;
	end = origin + ray;

	float tl, tr, bl, br;
	tl = (end[1]-start[1])*topLeft[0] + (start[0]-end[0])*topLeft[1]
		+ (end[0]*start[1] - start[0]*end[1]);
	tr = (end[1]-start[1])*topRight[0] + (start[0]-end[0])*topRight[1]
		+ (end[0]*start[1] - start[0]*end[1]);
	bl = (end[1]-start[1])*bottomLeft[0] + (start[0]-end[0])*bottomLeft[1]
		+ (end[0]*start[1] - start[0]*end[1]);
	br = (end[1]-start[1])*bottomRight[0] + (start[0]-end[0])*bottomRight[1]
		+ (end[0]*start[1] - start[0]*end[1]);

	if ((tl > 0 && tr > 0 && bl > 0 && br > 0) || !(tl > 0 || tr > 0 || bl > 0 || br > 0))
		return false;

	if ((start[0] > topRight[0] && end[0] > topRight[0]) ||
		(start[0] < bottomLeft[0] && end[0] < bottomLeft[0]) ||
		(start[1] > topRight[1] && end[1] > topRight[1]) ||
		(start[1] < bottomLeft[1] && end[1] < bottomLeft[1]))
		return false;

	return true;
}