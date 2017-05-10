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
    deformer = new Chainmail(mesh, aMin, aMax, b);
}

Slime::Slime(float r) : Sphere(r) {
    usingStandardMesh = false;
    useCustomMesh();
    deformer = new Chainmail(mesh, aMin, aMax, b);
}

Slime::Slime(float r, glm::vec3 p) : Sphere(r, p) {
    usingStandardMesh = false;
    useCustomMesh();
    deformer = new Chainmail(mesh, aMin, aMax, b);
}

void Slime::moveBy(float x, float y, float z, double dt) {
    deformer->simStep(0, glm::vec3(x, y, z), dt);
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

void Slime::moveBy(glm::vec3 t, double dt) {
    deformer->applyMove(0, t, dt);
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
