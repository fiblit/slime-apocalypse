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
    int enemies = enemyObjects.size();
	for (int i = 0; i < enemies; i++) {
		delete enemyObjects[i];
	}
	enemyObjects.clear();
    int staticObjectsCount = staticObjects.size();
	for (int i = 0; i < staticObjectsCount; i++) {
		delete staticObjects[i];
	}
	staticObjects.clear();
}

void Scene::automatonSimulate() {
    int numIterations = 15;
    for (int a = 0; a < numIterations; a++) {
        std::vector<std::vector<mazeCell>> mazeBuffer;
        for (int i = 0; i < mazeInfo.width; i++) {
            std::vector<mazeCell> bufferRow;
            for (int j = 0; j < mazeInfo.height; j++) {
                if (maze[i][j].active) {
                    int neighbors;
                    mazeCell cBuffer = {};
                    cBuffer.active = 1;
                    neighbors += maze[(i - 1) % mazeInfo.width][(j - 1) % mazeInfo.height].filled;
                    neighbors += maze[(i) % mazeInfo.width][(j - 1) % mazeInfo.height].filled;
                    neighbors += maze[(i + 1) % mazeInfo.width][(j - 1) % mazeInfo.height].filled;
                    neighbors += maze[(i - 1) % mazeInfo.width][(j + 1) % mazeInfo.height].filled;
                    neighbors += maze[(i) % mazeInfo.width][(j + 1) % mazeInfo.height].filled;
                    neighbors += maze[(i + 1) % mazeInfo.width][(j + 1) % mazeInfo.height].filled;
                    neighbors += maze[(i + 1) % mazeInfo.width][(j) % mazeInfo.height].filled;
                    neighbors += maze[(i - 1) % mazeInfo.width][(j) % mazeInfo.height].filled;
                    cBuffer.filled = (neighbors <= 1 || neighbors >= 4) ? 0 : 1;
                    bufferRow.push_back(cBuffer);
                }
                else {
                    bufferRow.push_back(maze[i][j]);
                }
            }
            mazeBuffer.push_back(bufferRow);
        }
        maze = mazeBuffer;
    }
    for (int i = 0; i < mazeInfo.width; i++) {
        for (int j = 0; j < mazeInfo.height; j++) {
            maze[i][j].active = 0;
        }
    }
    return;
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
    automatonSimulate();
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

    //trying to convert from worldspace to gridspace. Not sure if float -> int conversion works this way
    glm::ivec3 gridMoves = (newCenter - mazeInfo.center)/ mazeInfo.cellSize;
    //unload enemies that are outside of X by X grid & generate new ones
    int enemyObjectCount = enemyObjects.size();
    int idx= 0;
    while (idx< enemyObjectCount) {
        vec3 gridEnemyPos = snapToGrid(enemyObjects[idx]->dyn.pos);
        float xDist = abs(newCenter[0] - gridEnemyPos[0]);
        float yDist = abs(newCenter[1] - gridEnemyPos[1]);
        if (xDist > 15 || yDist > 15) {
            enemyObjects.erase(enemyObjects.begin() + idx);
            enemyObjectCount--;
            //TODO: Generate New Enemy Object
        }
        else {
            idx++;
        }
    }

    //shift the cells to match the new player center
    if (gridMoves[0] < 0) {
        for (int i = maze.size() - 1 + gridMoves[0]; i >= 0; i--) {
            maze[i] = maze[i - gridMoves[0]];
        }
    }
    else if (gridMoves[0] > 0){
        int shiftUpTo = maze.size() - gridMoves[0];
        for (int i = 0; i < shiftUpTo; i++) {
            maze[i] = maze[i + gridMoves[0]];
        }
    }
    if (gridMoves[1] < 0) {
        for (int i = 0; i < maze.size(); i++) {
            for (int j = maze[i].size() - 1 + gridMoves[1]; j >= 0; j--) {
                maze[i][j] = maze[i][j - gridMoves[1]];
            }
        }
    }
    else if (gridMoves[1] > 0){
        for (int i = 0; i < maze.size(); i++) {
            for (int j = 0; j < maze[i].size() - gridMoves[1]; j++) {
                maze[i][j] = maze[i][j + gridMoves[1]];
            }
        }
    }
    //generate new columns and rows.
    int xIterator = gridMoves[0] / abs(gridMoves[0]);
    int yIterator = gridMoves[1] / abs(gridMoves[1]);
    int xIdx = (maze.size() + gridMoves[0]) % maze.size() - 1;
    int yIdx = (maze[0].size() + gridMoves[1]) % maze[0].size() - 1;
    //update horizontal rows with newly generated cells
    while (xIdx >= 0 && xIdx < maze.size()) {
        for (int y = 0; y < maze[xIdx].size(); y++) {
            maze[xIdx][y].active = 1;
            maze[xIdx][y].filled = (rand() > mazeInfo.chanceGennedAlive) ? 1 : 0;
        }
        xIdx += xIterator;
    }
    //update vertical rows 
    while (yIdx >= 0 && yIdx < maze[0].size()) {
        for (int x = 0; x < maze.size(); x++) {
            maze[x][yIdx].active = 1;
            maze[x][yIdx].filled = (rand() > mazeInfo.chanceGennedAlive) ? 1 : 0;
        }
        yIdx += yIterator;
    }
    //run the simulation with the new active cells
    automatonSimulate();

    //add the Objects 
    for (int i = 0; i < maze.size(); i++) {
        for (int j = 0; j < maze[i].size(); j++) {
            if (maze[i][j].filled) {
                //add to staticObjects list
            }
        }
    }

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