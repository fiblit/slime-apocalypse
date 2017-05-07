#include "Scene.hpp"

using glm::vec3;
using std::vector;
using std::cout;
using std::endl;

Scene::Scene() {
	// Generate player object
	playerObject = new  Sphere(0.0, 0.0, 1.0);
	// Generate static objects (walls, floors, etc.)
    initMaze();
	// Generate enemy objects
	for (int i = 0; i < maxEnemies; i++) {
        
	}
	// Generate PRM per Agent's BV type
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

//use a cellular Automaton algorithm to initialize the maze.
//generate a randomly initialized grid and run through a fixed number of interations of the automaton algorithm
void Scene::initMaze() {
    //initialize the maze
    mazeInfo.height = 30;
    mazeInfo.width = 30;
    mazeInfo.chanceGennedAlive = .25;
    for (int i = 0; i < mazeInfo.width; i++) {
        std::vector<mazeCell> row;
        for (int j = 0; j < mazeInfo.height; j++) {
            mazeCell c = {};
            c.active = 1;
            c.filled = (rand() > mazeInfo.chanceGennedAlive) ? 1 : 0;
            row.push_back(c);
        }
        maze.push_back(row);
    }
    //player stays at center. should ALWAYS be empty;
    maze[14][14].filled = 0;
    //iterate through Conways game of life a fixed number of times.
    int numIterations = 15;
    for (int a = 0; a < numIterations; a++) {
        std::vector<std::vector<mazeCell>> mazeBuffer;
        for (int i = 0; i < mazeInfo.height; i++) {
            std::vector<mazeCell> bufferRow;
            for (int j = 0; j < mazeInfo.width; j++) {
                int neighbors;
                mazeCell cBuffer = {};
                if (i > 0 && j > 0) neighbors += maze[i - 1][j - 1].filled;
                if (j > 0) neighbors += maze[i][j - 1].filled;
                if (j > 0 && i < maze.size() - 1) neighbors += maze[i + 1][j - 1].filled;
                if (i < maze.size() - 1) neighbors += maze[i + 1][j].filled;
                if (j < maze[0].size() - 1 && i < maze.size() - 1) neighbors += maze[i + 1][j + 1].filled;
                if (j < maze[0].size() - 1) neighbors += maze[i][j + 1].filled;
                if (j < maze[0].size() - 1 && i > 0) neighbors += maze[i - 1][j + 1].filled;
                if (i > 0) neighbors += maze[i - 1][j].filled;
                cBuffer.filled = (neighbors <= 1 || neighbors >= 4) ? 0 : 1;
                bufferRow.push_back(cBuffer);
            }
            mazeBuffer.push_back(bufferRow);
        }
        maze = mazeBuffer;
    }
    //remove generated cells from active set. This is the 'play area' and things should be static within this area around the player.
    for (int i = 0; i < mazeInfo.height; i++) {
        for (int j = 0; j < mazeInfo.width; j++) {
            maze[i][j].active = 0;
        }
    }
    //put the nearby cells that have objects in to the staticObjects vector
    return;
}
//helper function that takes coordinates and returns the nearest grid center coordinate.
vec3 Scene::snapToGrid(vec3 pos) {
    float xLow = (int)(pos[0] / mazeInfo.cellSize) * mazeInfo.cellSize;
    if (xLow < 0) xLow = xLow - mazeInfo.cellSize;
    float xHigh = xLow + mazeInfo.cellSize;
    float yLow = (int)(pos[1] / mazeInfo.cellSize) * mazeInfo.cellSize;
    if (yLow < 0) yLow = yLow - mazeInfo.cellSize;
    float yHigh = yLow + mazeInfo.cellSize;
    float xSnap = (abs(xLow - pos[0]) < abs(xHigh - pos[0])) ? xLow : xHigh;
    float ySnap = (abs(yLow - pos[1]) < abs(yHigh - pos[1])) ? yLow : yHigh;

    return vec3(xSnap, ySnap, 0);
}
//Take the player position and generate maze cells for any cells in the X by X grid surrounding the player that don't
//have cells generated.
//TODO: Possibly have it hold on to pre-generated cells, only load static objects associated with cells in nearby area.
void Scene::generateMoreMaze(){
    vec3 newCenter = snapToGrid(playerObject->dyn.pos);
    //Determine how many cells the player has moved and shift the cells in the maze to match. Toss out any on the edge and generate new ones on the opposite edge
    //Convert from world coordinates to estimates
    glm::vec3 gridMoves = newCenter - vec3(mazeInfo.center)/ mazeInfo.cellSize;

    //unload enemies that are outside of X by X grid & generate new ones


    //set position of static objects to match nearest objects in maze(don't need to delete/reload, only update positions)
    

    //add PRM nodes that exist in newly generated maze area

    //re-run pathfinding algorithms
    return;
}



void Scene::addEnemyObject(float r, float x, float y, float z) {
	// Instantiate an enemy object

	// Add instantiated object to enemyObjects vector
}

void Scene::addRect(float h, float x1, float y1, float x2, float y2) {
	// Instantiate a wall object

	// Add instantiated object to staticObjects vector

}

void Scene::addCylinder(float h, float r, float x, float y){
	//Instantiate Cylinder
	
	//Add object to staticObjects vector
	
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
void Scene::setBounds(vec3 min, vec3 max) {
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

void Scene::simulate(double dt) {
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