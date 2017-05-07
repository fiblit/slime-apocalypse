#ifndef SCENE_H_GUARD
#define SCENE_H_GUARD

#include <vector>
#include <iostream>
#include "Object.hpp"
#include "Structs.hpp"
#include "Sphere.hpp"
#include "Rectangle.hpp"

#pragma warning(push, 0)
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#pragma warning(pop)

class Scene {
public:
	Scene();
	virtual ~Scene();

	void addEnemyObject(float r, float x, float y, float z);
	void addRect(float h, float x1, float y1, float x2, float y2);
	void addCylinder(float h, float r, float x, float y);
	void setPlayerColor(float r, float g, float b);
	void setPlayerColor(glm::vec3 rgb);
    void initMaze();
    void generateMoreMaze();
	void updatePRM();

	void setBounds(glm::vec3 min, glm::vec3 max);
	
	void simulate(double dt);

	void render();
	Object *playerObject;
	float maxEnemies;
	std::vector<Object *> enemyObjects;
	float maxObstacles;
	std::vector<Object *> staticObjects;
    mazeData mazeInfo;
    std::vector<std::vector<mazeCell>> maze;
	//PRM prm;
	bufferContainer bc;

private:

    glm::vec3 snapToGrid(glm::vec3 position);
};

#endif // SCENE_H_GUARD