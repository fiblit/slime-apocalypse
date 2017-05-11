#include "Slime.hpp"

//the deformable object, based on the sphere.

Slime::Slime()  : Sphere(){
    params[0] = 1.0;
    params[1] = 1.0;
    params[2] = 1.0;
    constructStandardMesh();
}

Slime::Slime(float r, float x, float y, float z) : Sphere(r, x, y, z) {
    usingStandardMesh = false;
    useCustomMesh();
    deformer = new Chainmail(mesh, stacks, slices);
}

Slime::Slime(float r) : Sphere(r) {
    usingStandardMesh = false;
    useCustomMesh();
    deformer = new Chainmail(mesh, stacks, slices);
}

Slime::Slime(float r, glm::vec3 p) : Sphere(r, p) {
    usingStandardMesh = false;
    useCustomMesh();
    deformer = new Chainmail(mesh, stacks, slices);
}

void Slime::simulate(float dt) {
	deformer->simStep(0, glm::vec3(0), dt);
}

void Slime::moveBy(float x, float y, float z) {
	moveBy(0, glm::vec3(x,y,z), 0);
}

void Slime::moveBy(float x, float y, float z, double dt) {
    moveBy(0, glm::vec3(x,y,z), dt);
}

void Slime::moveBy(int id, glm::vec3 t, double dt) {
    deformer->simStep(id, t, dt);
    std::vector<glm::vec3> v;
    deformer->returnVertices(v);
    std::vector<Vertex> newMeshVertices;
    for (int i = 0; i < v.size(); i++) {
        Vertex newVert = {};
        newVert.Position = v[i];
        newMeshVertices.push_back(newVert);
    }
    mesh->updateVertices(newMeshVertices);
    mesh->updateNormals();
}

void Slime::moveBy(std::vector<int> ids, glm::vec3 t, double dt) {
	deformer->simStep(ids, t, dt);
	std::vector<glm::vec3> v;
	deformer->returnVertices(v);
	std::vector<Vertex> newMeshVertices;
	for (int i = 0; i < v.size(); i++) {
		Vertex newVert = {};
		newVert.Position = v[i];
		newMeshVertices.push_back(newVert);
	}
	mesh->updateVertices(newMeshVertices);
	mesh->updateNormals();
}

void Slime::moveBy(glm::vec3 t) {
    deformer->applyMove(0, t);
    std::vector<glm::vec3> v;
    deformer->returnVertices(v);
    std::vector<Vertex> newMeshVertices;
    for (int i = 0; i < v.size(); i++) {
        Vertex newVert = {};
        newVert.Position = v[i];
        newMeshVertices.push_back(newVert);
    }
    mesh->updateVertices(newMeshVertices);
    mesh->updateNormals();
}
void Slime::moveTo(float x, float y, float z) {
    deformer->applyMove(0, glm::vec3(x, y, z));
}
void Slime::moveTo(glm::vec3 position) {
    deformer->applyMove(0, position);
}

Slime::~Slime()
{
}
