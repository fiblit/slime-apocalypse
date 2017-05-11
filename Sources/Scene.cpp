#include "Scene.hpp"
#include "debug.hpp"

using namespace glm;
using namespace std;

Scene::Scene() {
    this->camera = new Camera();
    camera->pos = vec3(5, 5, 5);
    camera->dir = vec3(0,1, 0);
    this->floor = new Cube(10000, .5, 10000, vec3(0, 0, -.5));
	// Generate player object
    
	playerObject = new  Sphere(1, 0,0,0);
	// Generate static objects (walls, floors, etc.)
    mazeInfo.height = 20;
    mazeInfo.width = 20;
    mazeInfo.maxEnemies = 5;
    mazeInfo.enemySize = 1;
    mazeInfo.chanceGennedAlive = .5;
    mazeInfo.cellSize = 10;
    mazeInfo.center = playerObject->dyn.pos;
    initMaze();
	// Generate enemy objects
    fillEnemyVector();
	// Generate PRM per Agent's BV type
    
}

void Scene::fillEnemyVector(int start, int end, bool colsFlag) {
    int i = enemyObjects.size();
    while (i < mazeInfo.maxEnemies) {
        //We're looking at columns [X][]
        if (colsFlag) {
            int randXGrid = (int)((float)rand()) / RAND_MAX * (start - end);
            int randYGrid = (int)((float)rand()) / RAND_MAX * (mazeInfo.height-1);
            int attempts = 0;
            while (maze[randXGrid][randYGrid].filled || (randXGrid == mazeInfo.width / 2 && mazeInfo.height / 2) && attempts < 10) {
                randXGrid = (int)((float)rand()) / RAND_MAX * (start - end);
                randYGrid = (int)((float)rand()) / RAND_MAX * (mazeInfo.height-1);
                attempts++;
            }
        }
        //otherwise it's in rows maze[][X];
        else {
            for (int i = start; i < end; i++) {
                int randXGrid = (int)((float)rand()) / RAND_MAX * (mazeInfo.width-1);
                int randYGrid = (int)((float)rand()) / RAND_MAX * (start - end);
                int attempts = 0;
                while (maze[randXGrid][randYGrid].filled || (randXGrid == mazeInfo.width / 2 && mazeInfo.height / 2) && attempts < 10) {
                    randXGrid = (int)(((float)rand()) / RAND_MAX * (mazeInfo.width-1));
                    randYGrid = (int)((float)rand()) / RAND_MAX * (start - end);
                    attempts++;
                }
            }
        }
        i++;
    }
}

void Scene::fillEnemyVector() {
    int i = enemyObjects.size();
    while (i < mazeInfo.maxEnemies) {
        int randXGrid = (int)(((float)rand()) / RAND_MAX) * (mazeInfo.width-1);
        int randYGrid = (int)(((float)rand()) / RAND_MAX) * (mazeInfo.height-1);
        while (maze[randXGrid][randYGrid].filled || (randXGrid == mazeInfo.width / 2 && randYGrid == mazeInfo.height / 2)) {
            randXGrid = (int)(((float)rand()) / RAND_MAX) * (mazeInfo.width-1);
            randYGrid = (int)(((float)rand()) / RAND_MAX) * (mazeInfo.height-1);
        }
        float worldX = (randXGrid - mazeInfo.width / 2)* mazeInfo.cellSize;
        float worldY = (randYGrid - mazeInfo.height / 2)* mazeInfo.cellSize;
        addEnemyObject(mazeInfo.enemySize, worldX, 0, worldY);
        i++;
    }
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


int Scene::mod(int num1, int num2) {
    int r = num1 % num2;
    return (r < 0) ? r + num2 : r;
}
void Scene::automatonSimulate() {
    int numIterations = 15;
    for (int a = 0; a < numIterations; a++) {
        std::vector<std::vector<mazeCell>> mazeBuffer;
        for (int i = 0; i < mazeInfo.width; i++) {
            std::vector<mazeCell> bufferRow;
            for (int j = 0; j < mazeInfo.height; j++) {
                if (maze[i][j].active) {
                    int neighbors = 0;
                    mazeCell cBuffer = {};
                    cBuffer.active = 1;
                    neighbors += maze[mod((i - 1), mazeInfo.width)][mod(j - 1, mazeInfo.height)].filled;
                    neighbors += maze[mod(i, mazeInfo.width)][mod(j - 1, mazeInfo.height)].filled;
                    neighbors += maze[mod((i + 1), mazeInfo.width)][mod((j - 1),mazeInfo.height)].filled;
                    neighbors += maze[mod((i - 1), mazeInfo.width)][mod((j + 1), mazeInfo.height)].filled;
                    neighbors += maze[mod(i,mazeInfo.width)][mod((j + 1),mazeInfo.height)].filled;
                    neighbors += maze[mod((i + 1), mazeInfo.width)][mod((j + 1), mazeInfo.height)].filled;
                    neighbors += maze[mod((i + 1), mazeInfo.width)][mod(j, mazeInfo.height)].filled;
                    neighbors += maze[mod((i - 1), mazeInfo.width)][mod(j, mazeInfo.height)].filled;
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

void Scene::fillStaticObjVector() {
    for (int i = 0; i < staticObjects.size(); i++) {
        delete staticObjects[i];
    }
    staticObjects.clear();
    for (size_t i = 0; i < maze.size(); i++) {
        for (size_t j = 0; j < maze[i].size(); j++) {
            if (maze[i][j].filled) {
                float xPos = (i - (mazeInfo.width / 2.0)) * mazeInfo.cellSize;
                float yPos = (j - (mazeInfo.height / 2.0)) * mazeInfo.cellSize;
                vec3 pos = mazeInfo.center + vec3(xPos, 0, yPos);
                addWall(10, mazeInfo.cellSize, mazeInfo.cellSize, pos);
            }
            maze[i][j].active = 0;
        }
    }
}
//use a cellular Automaton algorithm to initialize the maze.
//generate a randomly initialized grid and run through a fixed number of interations of the automaton algorithm
void Scene::initMaze() {
    mazeInfo.center = playerObject->dyn.pos;
    //initialize the maze
    for (int i = 0; i < mazeInfo.width; i++) {
        std::vector<mazeCell> row;
        for (int j = 0; j < mazeInfo.height; j++) {
            mazeCell c = {};
            c.active = 1;
            float randNum = ((float)rand()) / RAND_MAX;
            c.filled = (randNum > mazeInfo.chanceGennedAlive) ? 1 : 0;
            row.push_back(c);
        }
        maze.push_back(row);
    }
    //player stays at center. should ALWAYS be empty;
    maze[(mazeInfo.width / 2)][(mazeInfo.height / 2)].filled = 0;
    //iterate through Conways game of life a fixed number of times.
    automatonSimulate();
    maze[mazeInfo.width / 2][mazeInfo.height / 2].filled = 0;
    fillStaticObjVector();
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
    
    float zLow = (int)(pos[2] / mazeInfo.cellSize) * mazeInfo.cellSize;
    if (zLow < 0) zLow = zLow - mazeInfo.cellSize;
    float zHigh = zLow + mazeInfo.cellSize;

    float xSnap = (abs(xLow - pos[0]) < abs(xHigh - pos[0])) ? xLow : xHigh;
    float ySnap = (abs(yLow - pos[1]) < abs(yHigh - pos[1])) ? yLow : yHigh;
    float zSnap = (abs(zLow - pos[2]) < abs(zHigh - pos[2])) ? zLow : zHigh;

    return vec3(xSnap, ySnap, zSnap);
}




//Take the player position and generate maze cells for any cells in the X by X grid surrounding the player that don't
//have cells generated.
//TODO: Possibly have it hold on to pre-generated cells, only load static objects associated with cells in nearby area.
void Scene::generateMoreMaze() {
    vec3 newCenter = snapToGrid(playerObject->dyn.pos);
    //Determine how many cells the player has moved and shift the cells in the maze to match. Toss out any on the edge and generate new ones on the opposite edge
    //Convert from world coordinates to estimates

    //trying to convert from worldspace to gridspace. Not sure if float -> int conversion works this way
    glm::ivec3 gridMoves = (newCenter - mazeInfo.center) / mazeInfo.cellSize;
    /*
    cout << gridMoves[0] << "," << gridMoves[1] << ", " << gridMoves[2] << endl;
    cout << newCenter[0] << "," << newCenter[1] << ", " << newCenter[2] << endl;
    cout << mazeInfo.center[0] << "," << mazeInfo.center[1] << ", " << mazeInfo.center[2] << endl;
    */
    mazeInfo.center = newCenter;
    //unload enemies that are outside of X by X grid & generate new ones
    int enemyObjectCount = enemyObjects.size();
    int idx = 0;
    while (idx < enemyObjectCount) {
        vec3 gridEnemyPos = snapToGrid(enemyObjects[idx]->dyn.pos);
        float xDist = abs(newCenter[0] - gridEnemyPos[0]);
        float yDist = abs(newCenter[2] - gridEnemyPos[2]);
        if (xDist > mazeInfo.width / 2 || yDist > mazeInfo.height / 2) {
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
        int rightShift = -1 * gridMoves[0];
        for (int i = (maze.size() - rightShift - 1); i >= 0; i--) {
            maze[i - gridMoves[0]] = maze[i];
        }
    }
    else if (gridMoves[0] > 0){
        for (size_t i = 0; i < maze.size() - gridMoves[0]; i++) {
            maze[i] = maze[i + gridMoves[0]];
        }
    }
    if (gridMoves[2] < 0) {
        int downShift = -1 * gridMoves[2];
        for (int i = 0; i < maze.size(); i++) {
            for (int j = (maze[i].size() - downShift - 1); j >= 0; j--) {
                maze[i][j - gridMoves[2]] = maze[i][j];
            }
        }
    }
    else if (gridMoves[2] > 0){
        for (size_t i = 0; i < maze.size(); i++) {
            for (size_t j = 0; j < maze[i].size() - gridMoves[2]; j++) {
                maze[i][j] = maze[i][j + gridMoves[2]];
            }
        }
    }
    //generate new columns and rows.
    int xIterator = 0; 
    int yIterator = 0;
    if (gridMoves[0] != 0) {
        xIterator = gridMoves[0] / abs(gridMoves[0]);
        size_t xIdx = (maze.size() - gridMoves[0]) % maze.size();
        if (xIterator < 0) xIdx += xIterator;
        cout << xIdx << " " << xIterator << endl;
        //update horizontal rows with newly generated cells
        while (xIdx >= 0 && xIdx < maze.size()) {
            for (size_t y = 0; y < maze[xIdx].size(); y++) {
                maze[xIdx][y].active = 1;
                maze[xIdx][y].filled = (((float)rand())/RAND_MAX > mazeInfo.chanceGennedAlive) ? 1 : 0;
            }
            xIdx += xIterator;
        }
    }
    if (gridMoves[2] != 0) {
        yIterator = gridMoves[2] / abs(gridMoves[2]);
        size_t yIdx = (maze[0].size() - gridMoves[2]) % maze[0].size();
        if (yIterator < 0) yIdx += yIterator;
        //update vertical rows 
        while (yIdx >= 0 && yIdx < maze[0].size()) {
            for (size_t x = 0; x < maze.size(); x++) {
                maze[x][yIdx].active = 1;
                maze[x][yIdx].filled = (((float)rand())/RAND_MAX > mazeInfo.chanceGennedAlive) ? 1 : 0;
            }
            yIdx += yIterator;
        }
    }
    //run the simulation with the new active cells
    automatonSimulate();
    //add the Objects 
    fillStaticObjVector();
    if (gridMoves[0] != 0) {
        int start = (gridMoves[0] < 0) ? 0 : maze.size()-1;
        int end = (gridMoves[0] < 0) ? (-1 * gridMoves[0]) : (maze.size() - 1);
        assert(start > 0 && end < maze.size());
        //fillEnemyVector(start, end, true);
    }
    else if(gridMoves[2] != 0){
        int start = (gridMoves[2] < 0) ? 0 : (maze[0].size() - 1);
        int end = (gridMoves[2] < 0) ? (-1 * gridMoves[2]) : (maze[0].size() - 1);
        assert(start > 0 && end << maze[0].size());
        //fillEnemyVector(start, end, false);

    }
    //add PRM nodes that exist in newly generated maze area

    //re-run pathfinding algorithms
    return;
}



void Scene::addEnemyObject(float r, float x, float y, float z) {
	// Instantiate an enemy object
	Slime * enemy = new Slime(r, x, y, z);

	// Add instantiated object to enemyObjects vector
	enemyObjects.push_back(enemy);
}

void Scene::addWall(float h, float x1, float x2, float z1, float z2) {
	// Instantiate a wall object
	Cube * wall = new Cube(abs(x1-x2), abs(z1-z2), h, (x1+x2)/2, 0.0, (z1+z2)/2);

    // Add instantiated object to staticObjects vector
    staticObjects.push_back(wall);

    // Modify and/or rebuild PRM
    // ???
}

void Scene::addWall(float h, float w, float l, vec3 center) {
    // Instantiate a wall object
    Cube * wall = new Cube(w, l, h, center);

    // Add instantiated object to staticObjects vector
    staticObjects.push_back(wall);

    // Modify and/or rebuild PRM
    // ???
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
//in fact I would replace this with a call to the PRM constructor
void Scene::setBounds(vec3 min, vec3 max) {
    UNUSED(min, max);
	// Set the sampling space of the PRM
}

//note for later: should this be renamed to animate_dynamics?
//the actual simulation is kinda spread out, particularly in force-based functions
//like the stuff in the LMP.
void Scene::simulate(GLfloat dt) {
    // For each dynamic object:
    for (Object * o : enemyObjects) {
        //Forward euler integration of motion
        o->dyn.vel += o->dyn.force * dt;
        o->dyn.vel += o->dyn.gravity * dt;
        //o->dyn.pos += o->dyn.vel * dt;
        o->moveBy(o->dyn.vel * dt);//has side effects of changing dyn->pos

        //TODO dalton: notify other systems of events such as collisions (for particle effects, merging, etc.)

        //Reset forces for next frame
        o->dyn.force = glm::vec3(0);
    }

    // Move the player character? (I don't know if we should have that logic in the scene)
    // All UI does is say that the player should be moving somebody needs to actually move it
    // However, it might be best to have a preceding function for handling input
    playerObject->dyn.vel += playerObject->dyn.force * dt;
    //playerObject->dyn.pos += playerObject->dyn.vel * dt;
    playerObject->moveBy(playerObject->dyn.vel * dt);//has side effects of changing dyn->pos
    playerObject->dyn.force = glm::vec3(0);

	// Relax the chainmails
	for (Slime * o : enemyObjects) {
		o->simulate(dt);
	}
}

void Scene::render() {
	// Render the player object
    this->playerObject->draw(curShader);

    //render Floor
    floor->draw(curShader);
	// Render each enemy object
    for (Object * o : enemyObjects)
        o->draw(curShader);

	// Render each static object
	for (Object * o : staticObjects)
		o->draw(curShader);

	// Render the UI (if we have one)
	// maybe if I (dalton) add on dear-imgui later
}

void Scene::enableTextureShader(glm::mat4 proj, glm::mat4 view) {
	glm::mat4 model;

	shaders[TEXTURE]->enable();
	curShader = shaders[TEXTURE];

	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_container);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex_container_specular);*/

	//glUniform3f(shaders[TEXTURE]->uniform("material.ambient"), 1.0f, 0.5f, 0.31f);
	glUniform1f(shaders[TEXTURE]->uniform("material.shine"), 32.0f);
	glUniform1i(shaders[TEXTURE]->uniform("material.diffuse"), 0);
	glUniform1i(shaders[TEXTURE]->uniform("material.specular"), 1);

	glUniform3f(shaders[TEXTURE]->uniform("dirLight.direction"), dir_light_dir.x, -dir_light_dir.y, dir_light_dir.z);
	glUniform3f(shaders[TEXTURE]->uniform("dirLight.ambient"), light_ambient.x*0.1f, light_ambient.y*0.1f, light_ambient.z*0.1f);
	glUniform3f(shaders[TEXTURE]->uniform("dirLight.diffuse"), light_diffuse.x*0.1f, light_diffuse.y*0.1f, light_diffuse.z*0.1f);
	glUniform3f(shaders[TEXTURE]->uniform("dirLight.specular"), light_specular.x*0.1f, light_specular.y*0.1f, light_specular.z*0.1f);

	for (GLuint i = 0; i < 4; i++) {
		std::string si = "pointLights[" + std::to_string(i) + "].";
		glUniform3f(shaders[TEXTURE]->uniform(si + "position"), point_light_positions[i].x, point_light_positions[i].y, point_light_positions[i].z);
		glUniform1f(shaders[TEXTURE]->uniform(si + "constant"), 1.0f);
		glUniform1f(shaders[TEXTURE]->uniform(si + "linear"), 0.045f);
		glUniform1f(shaders[TEXTURE]->uniform(si + "quadratic"), 0.0075f);
		glUniform3f(shaders[TEXTURE]->uniform(si + "ambient"), light_ambient.x, light_ambient.y, light_ambient.z);
		glUniform3f(shaders[TEXTURE]->uniform(si + "diffuse"), light_diffuse.x, light_diffuse.y, light_diffuse.z);
		glUniform3f(shaders[TEXTURE]->uniform(si + "specular"), light_specular.x, light_specular.y, light_specular.z);
	}

	glUniform3f(shaders[TEXTURE]->uniform("spotLight.position"), camera->pos.x, camera->pos.y, camera->pos.z);
	glUniform3f(shaders[TEXTURE]->uniform("spotLight.direction"), camera->dir.x, camera->dir.y, camera->dir.z);
	glUniform3f(shaders[TEXTURE]->uniform("spotLight.ambient"), light_ambient.x*is_flashlight_on, light_ambient.y*is_flashlight_on, light_ambient.z*is_flashlight_on);
	glUniform3f(shaders[TEXTURE]->uniform("spotLight.diffuse"), light_diffuse.x*is_flashlight_on, light_diffuse.y*is_flashlight_on, light_diffuse.z*is_flashlight_on);
	glUniform3f(shaders[TEXTURE]->uniform("spotLight.specular"), light_specular.x*is_flashlight_on, light_specular.y*is_flashlight_on, light_specular.z*is_flashlight_on);
	glUniform1f(shaders[TEXTURE]->uniform("spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
	glUniform1f(shaders[TEXTURE]->uniform("spotLight.fadeOff"), glm::cos(glm::radians(17.5f)));
	glUniform1f(shaders[TEXTURE]->uniform("spotLight.constant"), 1.0f);
	glUniform1f(shaders[TEXTURE]->uniform("spotLight.linear"), 0.007f);
	glUniform1f(shaders[TEXTURE]->uniform("spotLight.quadratic"), 0.0002f);

	glUniformMatrix4fv(shaders[TEXTURE]->uniform("proj"), 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(shaders[TEXTURE]->uniform("view"), 1, GL_FALSE, glm::value_ptr(view));
}

void Scene::enableFlatShader(glm::mat4 proj, glm::mat4 view) {
	shaders[FLAT]->enable();
	curShader = shaders[FLAT];

	//glUniform3f(shaders[FLAT]->uniform("material.ambient"), 1.0f, 0.5f, 0.31f);
	glUniform1f(shaders[FLAT]->uniform("material.shine"), 32.0f);

	glUniform3f(shaders[FLAT]->uniform("dirLight.direction"), dir_light_dir.x, dir_light_dir.y, dir_light_dir.z);
	glUniform3f(shaders[FLAT]->uniform("dirLight.ambient"), light_ambient.x*0.1f, light_ambient.y*0.1f, light_ambient.z*0.1f);
	glUniform3f(shaders[FLAT]->uniform("dirLight.diffuse"), light_diffuse.x*0.1f, light_diffuse.y*0.1f, light_diffuse.z*0.1f);
	glUniform3f(shaders[FLAT]->uniform("dirLight.specular"), light_specular.x*0.1f, light_specular.y*0.1f, light_specular.z*0.1f);

	for (GLuint i = 0; i < 4; i++) {
		std::string si = "pointLights[" + std::to_string(i) + "].";
		glUniform3f(shaders[FLAT]->uniform( si + "position"), point_light_positions[i].x, point_light_positions[i].y, point_light_positions[i].z);
		glUniform1f(shaders[FLAT]->uniform( si + "constant"), 1.0f);
		glUniform1f(shaders[FLAT]->uniform( si + "linear"), 0.005f);
		glUniform1f(shaders[FLAT]->uniform( si + "quadratic"), 0.0005f);
		glUniform3f(shaders[FLAT]->uniform( si + "ambient"), light_ambient.x, light_ambient.y, light_ambient.z);
		glUniform3f(shaders[FLAT]->uniform( si + "diffuse"), light_diffuse.x, light_diffuse.y, light_diffuse.z);
		glUniform3f(shaders[FLAT]->uniform( si + "specular"), light_specular.x, light_specular.y, light_specular.z);
	}

	glUniform3f(shaders[FLAT]->uniform( "spotLight.position"), camera->pos.x, camera->pos.y, camera->pos.z);
	glUniform3f(shaders[FLAT]->uniform( "spotLight.direction"), camera->dir.x, camera->dir.y, camera->dir.z);
	glUniform3f(shaders[FLAT]->uniform( "spotLight.ambient"), light_ambient.x*is_flashlight_on, light_ambient.y*is_flashlight_on, light_ambient.z*is_flashlight_on);
	glUniform3f(shaders[FLAT]->uniform( "spotLight.diffuse"), light_diffuse.x*is_flashlight_on, light_diffuse.y*is_flashlight_on, light_diffuse.z*is_flashlight_on);
	glUniform3f(shaders[FLAT]->uniform( "spotLight.specular"), light_specular.x*is_flashlight_on, light_specular.y*is_flashlight_on, light_specular.z*is_flashlight_on);
	glUniform1f(shaders[FLAT]->uniform( "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
	glUniform1f(shaders[FLAT]->uniform( "spotLight.fadeOff"), glm::cos(glm::radians(17.5f)));
	glUniform1f(shaders[FLAT]->uniform( "spotLight.constant"), 1.0f);
	glUniform1f(shaders[FLAT]->uniform( "spotLight.linear"), 0.001f);
	glUniform1f(shaders[FLAT]->uniform( "spotLight.quadratic"), 0.00001f);

	glUniformMatrix4fv(shaders[FLAT]->uniform("proj"), 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(shaders[FLAT]->uniform("view"), 1, GL_FALSE, glm::value_ptr(view));

	glUniform3f(shaders[FLAT]->uniform("material.diffuse"), 1, 0, 0);
	glUniform3f(shaders[FLAT]->uniform("material.specular"), 1, 1, 1);
}

void Scene::enableLightShader(glm::mat4 proj, glm::mat4 view) {
	shaders[LIGHT]->enable();
	curShader = shaders[LIGHT];

	glUniformMatrix4fv(shaders[LIGHT]->uniform("proj"), 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(shaders[LIGHT]->uniform("view"), 1, GL_FALSE, glm::value_ptr(view));

	glUniform3f(shaders[LIGHT]->uniform("lightColor"), light_specular.x, light_specular.y, light_specular.z);
}

void Scene::enableTestShader(glm::mat4 proj, glm::mat4 view) {
	shaders[TEST]->enable();
	curShader = shaders[TEST];

	glUniformMatrix4fv(shaders[TEST]->uniform("proj"), 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(shaders[TEST]->uniform("view"), 1, GL_FALSE, glm::value_ptr(view));

    /*
	glUniform3f(shaders[TEST]->uniform("material.diffuse"), 1, 0, 0);
	glUniform3f(shaders[TEST]->uniform("material.specular"), 1, 1, 1);
	glUniform1f(shaders[TEST]->uniform("material.shine"), 32.0f);
    */
}
void Scene::setupTestingObjects() {
	// walls
	playerObject = new Sphere(1, 100, 0, 0);
    test = new Slime(3, 0, 0,  4);
    enemyObjects.push_back(test);
}

void Scene::toggle_flashlight() {
    is_flashlight_on = !is_flashlight_on;
}

void Scene::slimeTestMove() {
    test->dyn.force = vec3(10000, 0, 10000);
}


void Scene::slimeTestStill() {
    test->dyn.force = vec3(0,0,-1000);
    test->dyn.vel[0] = 0;
}