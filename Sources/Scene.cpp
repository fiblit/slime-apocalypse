#include "Scene.hpp"

using glm::vec3;
using std::vector;
using std::cout;
using std::endl;
using std::min;
using std::max;
using std::abs;

Scene::Scene() {
    this->camera = new Camera();

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
	// Instantiate an enemy object
	Sphere * enemy = new Sphere(x, y, z, r);

	// Add instantiated object to enemyObjects vector
	enemyObjects.push_back(enemy);
}

void Scene::addWall(float h, float x1, float y1, float x2, float y2) {
	// Instantiate a wall object
	Cube * wall = new Cube((x1+x2)/2, (y1+y2)/2, abs(x1-x2), abs(y1-y2), h);

	// Add instantiated object to staticObjects vector
	staticObjects.push_back(wall);

	// Modify and/or rebuild PRM
	// ???
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

//note for later: should this be renamed to animate_dynamics?
//the actual simulation is kinda spread out, particularly in force-based functions
//like the stuff in the LMP.
void Scene::simulate(GLfloat dt) {
    // For each dynamic object:
    for (Object * o : enemyObjects) {
        //Forward euler integration of motion
        o->dyn.vel += o->dyn.force * dt;
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
}

void Scene::render() {
	// Render the player object
    this->playerObject->draw(curShader);

	// Render each enemy object
    for (Object * o : enemyObjects)
        o->draw(curShader);

	// Render each static object
	for (Object * o : staticObjects)
		o->draw(curShader);

	// Render the UI (if we have one)
	// maybe if I (dalton) add on dear-imgui later
}

void Scene::enableTextureShader() {
	glm::mat4 proj = glm::perspective(glm::radians(camera->fov), (GLfloat)G::WIN_WIDTH / (GLfloat)G::WIN_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera->getView();
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

void Scene::enableFlatShader() {
	glm::mat4 proj = glm::perspective(glm::radians(camera->fov), (GLfloat)G::WIN_WIDTH / (GLfloat)G::WIN_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera->getView();

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
		glUniform1f(shaders[FLAT]->uniform( si + "linear"), 0.045f);
		glUniform1f(shaders[FLAT]->uniform( si + "quadratic"), 0.0075f);
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
	glUniform1f(shaders[FLAT]->uniform( "spotLight.linear"), 0.045f);
	glUniform1f(shaders[FLAT]->uniform( "spotLight.quadratic"), 0.0075f);

	glUniformMatrix4fv(shaders[FLAT]->uniform("proj"), 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(shaders[FLAT]->uniform("view"), 1, GL_FALSE, glm::value_ptr(view));

	glUniform3f(shaders[FLAT]->uniform("material.diffuse"), 1, 0, 0);
	glUniform3f(shaders[FLAT]->uniform("material.specular"), 1, 1, 1);
}

void Scene::enableLightShader() {
	glm::mat4 proj = glm::perspective(glm::radians(camera->fov), (GLfloat)G::WIN_WIDTH / (GLfloat)G::WIN_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera->getView();

	shaders[LIGHT]->enable();
	curShader = shaders[LIGHT];

	glUniformMatrix4fv(shaders[LIGHT]->uniform("proj"), 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(shaders[LIGHT]->uniform("view"), 1, GL_FALSE, glm::value_ptr(view));

	glUniform3f(shaders[LIGHT]->uniform("lightColor"), light_specular.x, light_specular.y, light_specular.z);
}

void Scene::enableTestShader() {
	glm::mat4 proj = glm::perspective(glm::radians(camera->fov), (GLfloat)G::WIN_WIDTH / (GLfloat)G::WIN_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera->getView();

	shaders[TEST]->enable();
	curShader = shaders[TEST];

	glUniformMatrix4fv(shaders[TEST]->uniform("proj"), 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(shaders[TEST]->uniform("view"), 1, GL_FALSE, glm::value_ptr(view));

	glUniform3f(shaders[TEST]->uniform("material.diffuse"), 1, 0, 0);
	glUniform3f(shaders[TEST]->uniform("material.specular"), 1, 1, 1);
	glUniform1f(shaders[TEST]->uniform("material.shine"), 32.0f);
}

void Scene::setupTestingObjects() {
	// walls
	addWall(2, -5, -6, 5, -8);
	addWall(3, 3, -8, 5, 8);
	addWall(1, -5, 6, 3, 8);
	addWall(3, -3, -3, -1, 3);

	// enemies
	addEnemyObject(1, 0, 0, 0);
	addEnemyObject(1, 2, -2, 0);
	addEnemyObject(2, -1, 3, 0);

	playerObject = new Sphere(0, -7, 6, 1);
}

void Scene::toggle_flashlight() {
    is_flashlight_on = !is_flashlight_on;
}
