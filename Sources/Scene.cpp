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


	// ** The following is uniform initializzation code for each shader; we'll have to figure out how we want to do this for everything that we render **/

	// shaders; pass uniforms; draw
	glm::mat4 proj = glm::perspective(glm::radians(camera->fov), (GLfloat)G::WIN_WIDTH / (GLfloat)G::WIN_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera->getView();
	glm::mat4 model;

	/* begin cube shaders */

	shaders[TEXTURE]->enable();

	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_container);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex_container_specular);*/

	glUniform3f(shaders[TEXTURE]->uniform("material.ambient"), 1.0f, 0.5f, 0.31f);
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

	/* end cube shaders */
	/* begin flat shaders */

	shaders[FLAT]->enable();

	glUniform3f(shaders[FLAT]->uniform("material.ambient"), 1.0f, 0.5f, 0.31f);
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

	glUniformMatrix4fv(shaders[FLAT]->uniform( "proj"), 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(shaders[FLAT]->uniform( "view"), 1, GL_FALSE, glm::value_ptr(view));

	for (GLuint i = 0; i < obj::NR_CUBES; i++) {
		glUniform3f(shaders[FLAT]->uniform("material.diffuse"), obj::cube_diffuse_color[i].x, obj::cube_diffuse_color[i].y, obj::cube_diffuse_color[i].z);
		glUniform3f(shaders[FLAT]->uniform("material.specular"), obj::cube_specular_color[i].x, obj::cube_specular_color[i].y, obj::cube_specular_color[i].z);
		model = glm::mat4();
		model = glm::translate(model, obj::cube_positions[i]);
		model = glm::scale(model, glm::vec3(obj::cube_scale[i]));
		glUniformMatrix4fv(shaders[FLAT]->uniform("model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(shaders[FLAT]->uniform( "normalMat"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(view * model))));

		//glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	/* end flat shaders */
	/* begin lamp shaders */

	shaders[LIGHT]->enable();

	glUniformMatrix4fv(shaders[LIGHT]->uniform( "proj"), 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(shaders[LIGHT]->uniform("view"), 1, GL_FALSE, glm::value_ptr(view));

	glUniform3f(shaders[LIGHT]->uniform("lightColor"), light_specular.x, light_specular.y, light_specular.z);

	for (GLuint i = 0; i < 4; i++) {
		model = glm::mat4();
		model = glm::translate(model, point_light_positions[i]);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(shaders[LIGHT]->uniform("model"), 1, GL_FALSE, glm::value_ptr(model));

		//glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	/* end lamp shaders */

}