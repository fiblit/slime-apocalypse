#include "Scene.hpp"

using glm::vec3;
using std::vector;
using std::cout;
using std::endl;

Scene::Scene() {
	// Generate player object
    // ?
	// Generate static objects (walls, floors, etc.)

	// Generate enemy objects

	// Generate PRM per Agent's BV type
}

Scene::~Scene() {
	if (playerObject)
		delete playerObject;

	for (size_t i = 0; i < enemyObjects.size(); i++) {
		delete enemyObjects[i];
	}
	enemyObjects.clear();

	for (size_t i = 0; i < staticObjects.size(); i++) {
		delete staticObjects[i];
	}
	staticObjects.clear();
}

#include "debug.hpp"
void Scene::addEnemyObject(float r, float x, float y, float z) {
    UNUSED(r, x, y, z);
	// Instantiate an enemy object

	// Add instantiated object to enemyObjects vector
}

void Scene::addWall(float h, float x1, float y1, float x2, float y2) {
    UNUSED(h,x1,y1,x2,y2);
	// Instantiate a wall object

	// Add instantiated object to staticObjects vector

	// Modify and/or rebuild PRM
}

void Scene::setPlayerColor(float r, float g, float b) {
	setPlayerColor(vec3(r, g, b));
}

void Scene::setPlayerColor(vec3 rgb) {
	playerObject->setColor(rgb);
}

// not sure what was intended for this
void Scene::updatePRM() {
	// Update necessary properties before updating PRM

	// Call PRM functions to update it
}
//may need to pass a handful more parameters to the PRM (e.g. how it samples)
//in fact I would replace this with a call to the PRM constructor
void Scene::setBounds(vec3 min, vec3 max) {
    UNUSED(min, max);
	// Set the sampling space of the PRM
}

/*
void Scene::replan(double dt) {
// for each Agent:
//		Update their target
//		Plan to target
//      Avoid local collisions
}
*/

//TODO dalton: let dalton clean up 1 thing in the LMP before adding this in.
void Scene::simulate(double dt) {
    UNUSED(dt);
	// For each enemy object:
    //      Integrate motion/dynamics 
    //      notify other systems of events such as collisions (for particle effects, merging, etc.)

	// Move the player character? (I don't know if we should have that logic in the scene)
    // All UI does is say that the player should be moving somebody needs to actually move it
    // However, it might be best to have a preceding function for handling input
}

void Scene::render() {
	// Render the player object
    this->playerObject->render(this->bc);
	// Render each enemy object
    for (Object * o : enemyObjects)
        o->render(this->bc);
	// Render each static object
    for (Object * o : staticObjects)
        o->render(this->bc);

	// Render the UI (if we have one)
    // maybe if I add on dear-imgui later
}