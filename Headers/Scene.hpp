#ifndef SCENE_H_GUARD
#define SCENE_H_GUARD

#include <vector>
#include <iostream>
#include "Object.hpp"
#include "Structs.hpp"

// Don't know how we bring these in from the top level
#include <glad/glad.h>// glad: an OpenGL function loader: https://github.com/Dav1dde/glad
#include <GLFW/glfw3.h>// glfw: a library for I/O and OpenGL context creation: http://www.glfw.org/
#include <glm/glm.hpp>// glm: OpenGL mathematics: http://glm.g-truc.net/0.9.8/index.html

using glm::vec3;
using std::vector;
using std::cout;
using std::endl;

class Scene {
public:
	Scene();
	virtual ~Scene();

	void addEnemyObject(float r, float x, float y, float z);
	void addWall(float h, float x1, float y1, float x2, float y2);

	void setPlayerColor(float r, float g, float b);
	void setPlayerColor(vec3 rgb);

	void updatePRM();

	void setBounds(vec3 min, vec3 max);

	void simulate(double dt);

	void render();

	Object *playerObject;
	vector<Object *> enemyObjects;
	vector<Object *> staticObjects;
	//PRM prm;
	bufferContainer bc;
};

#endif // SCENE_H_GUARD