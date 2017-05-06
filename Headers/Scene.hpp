#ifndef SCENE_H_GUARD
#define SCENE_H_GUARD

#include <vector>
#include <iostream>
#include <unordered_map>
#include "Object.hpp"
#include "Structs.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

#pragma warning(push, 0)
#pragma warning(disable: 4055)
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#pragma warning(disable: 4201)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma warning(pop)

enum shaderNames {
	TEXTURE,
	FLAT,
	LIGHT
};

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

	std::unordered_map<int, mcl::Shader *> shaders;

	Camera * camera;

	/* lighting */
	glm::vec3 light_diffuse = glm::vec3(0.5f, 0.5f, 0.5f); // Decrease the influence
	glm::vec3 light_ambient = glm::vec3(0.05f, 0.05f, 0.05f); // Low influence
	glm::vec3 light_specular = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 point_light_positions[4] = {
		glm::vec3(-8.f,  1.5f,  -8.f),
		glm::vec3(8.f,  1.5f, 8.f),
		glm::vec3(0.f,  1.f, 4.f),
		glm::vec3(5.f,  2.f, -5.f)
	};
	glm::vec3 dir_light_dir = glm::vec3(-0.5f, 1.0f, -0.7f);
	bool is_flashlight_on = true;
};

#endif // SCENE_H_GUARD