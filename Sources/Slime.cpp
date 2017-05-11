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
    deformer = new Chainmail(mesh, stacks, slices, this->dyn.pos);
}

Slime::Slime(float r) : Sphere(r) {
    usingStandardMesh = false;
    useCustomMesh();
    deformer = new Chainmail(mesh, stacks, slices, this->dyn.pos);
}

Slime::Slime(float r, glm::vec3 p) : Sphere(r, p) {
    usingStandardMesh = false;
    useCustomMesh();
    deformer = new Chainmail(mesh, stacks, slices, this->dyn.pos);
}

void Slime::simulate(float dt) {
	deformer->simStep(0, glm::vec3(0, 0, -1), dt);
    this->dyn.pos = deformer->returnWorldPos();
}

void Slime::moveBy(float x, float y, float z) {
	moveBy(glm::vec3(x,y,z), 0);
}

void Slime::moveBy(float x, float y, float z, double dt) {
    moveBy(glm::vec3(x,y,z), dt);
}


void Slime::moveBy(glm::vec3 t) {
    moveBy(t, 0);
}
void Slime::moveTo(float x, float y, float z) {
    deformer->applyMove(0, glm::vec3(x, y, z));
}
void Slime::moveTo(glm::vec3 position) {
    deformer->applyMove(0, position);
}

void Slime::moveBy(glm::vec3 t, double dt) {
    deformer->simStep(0, t, dt);
    std::vector<glm::vec3> v;
    deformer->returnVertices(v);
    std::vector<Vertex> newMeshVertices;
    for (int i = 0; i < v.size(); i++) {
        Vertex newVert = {};
        newVert.Position = v[i];
        newMeshVertices.push_back(newVert);
    }
    dyn.pos = deformer->returnWorldPos();
    mesh->updateVertices(newMeshVertices);
    mesh->updateNormals();

}

Slime::~Slime()
{
    delete mesh;
}
