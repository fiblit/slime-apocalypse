#include "Scene.hpp"

Scene::Scene() {
	// Generate player object

	// Generate static objects (walls, floors, etc.)

	// Generate enemy objects

	// Generate PRM
}

Scene::~Scene() {
	if (playerObject)
		delete playerObject;

	for (int i = 0; i < enemyObjects.size(); i++) {
		delete enemyObjects[i];
	}
	enemyObjects.clear();

	for (int i = 0; i < staticObjects.size(); i++) {
		delete staticObjects[i];
	}
	staticObjects.clear();
}

void Scene::addEnemyObject(float r, float x, float y, float z) {
	// Instantiate an enemy object

	// Add instantiated object to enemyObjects vector
}

void Scene::addWall(float h, float x1, float y1, float x2, float y2) {
	// Instantiate a wall object

	// Add instantiated object to staticObjects vector

	// Regenerate PRM
}

void Scene::setPlayerColor(float r, float g, float b) {
	setPlayerColor(vec3(r, g, b));
}

void Scene::setPlayerColor(vec3 rgb) {
	playerObject->setColor(rgb);
}

void Scene::updatePRM() {
	// Update necessary properties before updating PRM

	// Call PRM functions to update it
}

void Scene::setBounds(vec3 min, vec3 max) {
	// Set the sampling space of the PRM
}

void Scene::simulate(double dt) {
	// For each enemy object:
	//		Update their target
	//		Get direction to target
	//		Move in the direction of the target

	// Move the player character? (I don't know if we should have that logic in the scene)
}

void Scene::render() {
	// Render the player object

	// Render each enemy object

	// Render each static object

	// Render the UI (if we have one)
}