#ifndef SCENE_H_GUARD
#define SCENE_H_GUARD

#include <vector>
#include <iostream>
#include "Object.hpp"
#include "Structs.hpp"

#pragma warning(push, 0)
#pragma warning(disable: 4055)
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#pragma warning(disable: 4201)
#include <glm/glm.hpp>
#pragma warning(pop)

class Scene {
public:
	Scene();
	virtual ~Scene();

	void addEnemyObject(float r, float x, float y, float z);
	void addWall(float h, float x1, float y1, float x2, float y2);

	void setPlayerColor(float r, float g, float b);
	void setPlayerColor(glm::vec3 rgb);

	void updatePRM();

	void setBounds(glm::vec3 min, glm::vec3 max);

	void simulate(double dt);

	void render();

	Object *playerObject;
	std::vector<Object *> enemyObjects;
	std::vector<Object *> staticObjects;
	//PRM prm;
	bufferContainer bc;
};

#endif // SCENE_H_GUARD