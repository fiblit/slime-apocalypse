#include "main.hpp"

/* some of this might look like stuff from learnopengl.com ... probably not much anymore 
   I have been changing it a lot. 
   Oh, and I think there's a bit from github.com/Polytonic/glitter or maybe from the
   stb github repo... It's mostly just stuff to interface with libraries (e.g. GLFW, 
   stb, glad, etc. ) */
int main() {
    /* Load GLFW  */
	D(std::cout << "Initializing GLFW for OpenGL 3.3...");
    glfwInit();
	// Set required hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //For macs
	D(OK());

	/* Create Window Context */
	D(std::cout << "Creating GLFW Window Context...");
	//handle fullscreen
	GLFWmonitor* monitor = nullptr;
	if (G::WIN_FULLSCREEN)
		monitor = glfwGetPrimaryMonitor();
	//Make a window
	std::string title = "Dalton Hildreth ::: Homework 5";
    GLFWwindow* window = glfwCreateWindow(G::WIN_WIDTH, G::WIN_HEIGHT, title.c_str(), monitor, nullptr);
    if (window == nullptr) {
		std::cerr << "Failed to create OpenGL Context" << std::endl;
		return DIE(EXIT_FAILURE);
    }
	glfwMakeContextCurrent(window);
	D(OK());

	/* Define callbacks */
	D(std::cout << "Setting Callbacks...");
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	D(OK());

	/* load all OpenGL functions */
	D(std::cout << "Loading OpenGL with glad...");
	// via glad using the glfw loader function
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize OpenGL context" << std::endl;
		return DIE(EXIT_FAILURE);
	}
	// Alternative use the builtin loader, e.g. if no other loader function is available
	/*if (!gladLoadGL()) {
		std::cerr << "Failed to initialize OpenGL context" << std::endl;
		return DIE(EXIT_FAILURE);
	}*/
	D(std::cout << "OK ::: OpenGL " << glGetString(GL_VERSION) << std::endl);

	/* Handle Viewport */
	D(std::cout << "Creating viewport...");
	GLint width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	D(OK());

	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	/* Management */
	timer = new Timer();

	/* Shaders */
	auto shv = [](auto s) {return ; };
	auto shf = [](auto s) {return ((std::string)PROJECT_SOURCE_DIR + "/Shaders/" + s + ".frag").c_str(); };
	Shader* cubeShader = new Shader(
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/cube.vert").c_str(), 
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/cube.frag").c_str());
	Shader* flatShader = new Shader(
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/flat.vert").c_str(),
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/flat.frag").c_str());
	Shader* lampShader = new Shader(
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/lamp.vert").c_str(),
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/lamp.frag").c_str());
	cam = new Camera();

	/* Objects */
	// cubes
	const GLuint numBuf = 2;
	GLuint VAO[numBuf]; // should probably make this dynamically resizable... / managed
	glGenVertexArrays(numBuf, VAO);
	GLuint VBO[numBuf];
	glGenBuffers(numBuf, VBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(obj::cube), obj::cube, GL_STATIC_DRAW);
	// Position attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attr
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Tex Coords attr
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    // Position attr
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

	// lamps
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO, the container's VBO's data already contains the correct data.
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	// Position attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	/* Load textures */
	GLenum tex_format;
	GLint tex_width, tex_height, tex_channels;
	std::string RESOURCES_DIR = (std::string)PROJECT_SOURCE_DIR + "/Resources/";
	GLubyte * image = stbi_load((RESOURCES_DIR + "container2.png").c_str(), &tex_width, &tex_height, &tex_channels, 0);
	if (!image)
		std::cerr << "Failed to load texture " + RESOURCES_DIR + "container2.png" << std::endl;
	switch (tex_channels) {
		case 1: tex_format = GL_ALPHA;     break;
		case 2: tex_format = GL_LUMINANCE; break;
		case 3: tex_format = GL_RGB;       break;
		case 4: tex_format = GL_RGBA;      break;
	}

	GLuint tex_container;
	glGenTextures(1, &tex_container);
	glBindTexture(GL_TEXTURE_2D, tex_container);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, tex_format, tex_width, tex_height, 0, tex_format, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	image = stbi_load((RESOURCES_DIR+"container2_specular.png").c_str(), &tex_width, &tex_height, &tex_channels, 0);
	if (!image)
		std::cerr << "Failed to load texture "+RESOURCES_DIR+"container2_specular.png" << std::endl;
	switch (tex_channels) {
		case 1: tex_format = GL_ALPHA;     break;
		case 2: tex_format = GL_LUMINANCE; break;
		case 3: tex_format = GL_RGB;       break;
		case 4: tex_format = GL_RGBA;      break;
	}

	GLuint tex_container_specular;
	glGenTextures(1, &tex_container_specular);
	glBindTexture(GL_TEXTURE_2D, tex_container_specular);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, tex_format, tex_width, tex_height, 0, tex_format, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 1);

	/* Path Planning */
	init_planning();
	replan();

	//change properties for the path -- instantiate cube visualizations
	//init_planning_vis();

	/* lighting */
	glm::vec3 lightDiffuse = glm::vec3(0.5f, 0.5f, 0.5f); // Decrease the influence
	glm::vec3 lightAmbient = glm::vec3(0.05f, 0.05f, 0.05f); // Low influence
	glm::vec3 lightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 pointLightPositions[] = {
		glm::vec3(-8.f,  1.5f,  -8.f),
		glm::vec3(8.f,  1.5f, 8.f),
		glm::vec3(0.f,  1.f, 4.f),
		glm::vec3(5.f,  2.f, -5.f)
	};
	glm::vec3 dirLightDir = glm::vec3(-0.5f, 1.0f, -0.7f);
	isFlashlightOn = true;

    /* Game Loop */
	timer->tick();
	D(std::cout << std::endl << "Entering Game Loop..." << std::endl << std::endl);	
	while (!glfwWindowShouldClose(window)) {
		timer->tick();

		animate_agents(timer->getDelta());

		// Callbacks 
		glfwPollEvents();
		do_movement();

		// Render 
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// shaders; pass uniforms; draw
		glm::mat4 proj = glm::perspective(glm::radians(cam->fov), (GLfloat)G::WIN_WIDTH / (GLfloat)G::WIN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = cam->getView();
		glm::mat4 model;

		cubeShader->use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_container);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex_container_specular);

		glUniform3f(cubeShader->Uni("material.ambient"), 1.0f, 0.5f, 0.31f);
		glUniform1f(cubeShader->Uni("material.shine"), 32.0f);
		glUniform1i(cubeShader->Uni("material.diffuse"), 0);
		glUniform1i(cubeShader->Uni("material.specular"), 1);

		glUniform3f(cubeShader->Uni("dirLight.direction"), dirLightDir.x, -dirLightDir.y, dirLightDir.z);
		glUniform3f(cubeShader->Uni("dirLight.ambient"), lightAmbient.x*0.1f, lightAmbient.y*0.1f, lightAmbient.z*0.1f);
		glUniform3f(cubeShader->Uni("dirLight.diffuse"), lightDiffuse.x*0.1f, lightDiffuse.y*0.1f, lightDiffuse.z*0.1f);
		glUniform3f(cubeShader->Uni("dirLight.specular"), lightSpecular.x*0.1f, lightSpecular.y*0.1f, lightSpecular.z*0.1f);

		for (GLuint i = 0; i < 4; i++) {
			std::string si = "pointLights[" + std::to_string(i) + "].";
			glUniform3f(cubeShader->Uni(si + "position"), pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
			glUniform1f(cubeShader->Uni(si + "constant"), 1.0f);
			glUniform1f(cubeShader->Uni(si + "linear"), 0.045f);
			glUniform1f(cubeShader->Uni(si + "quadratic"), 0.0075f);
			glUniform3f(cubeShader->Uni(si + "ambient"), lightAmbient.x, lightAmbient.y, lightAmbient.z);
			glUniform3f(cubeShader->Uni(si + "diffuse"), lightDiffuse.x, lightDiffuse.y, lightDiffuse.z);
			glUniform3f(cubeShader->Uni(si + "specular"), lightSpecular.x, lightSpecular.y, lightSpecular.z);
		}

		glUniform3f(cubeShader->Uni("spotLight.position"), cam->pos.x, cam->pos.y, cam->pos.z);
		glUniform3f(cubeShader->Uni("spotLight.direction"), cam->dir.x, cam->dir.y, cam->dir.z);
		glUniform3f(cubeShader->Uni("spotLight.ambient"), lightAmbient.x*isFlashlightOn, lightAmbient.y*isFlashlightOn, lightAmbient.z*isFlashlightOn);
		glUniform3f(cubeShader->Uni("spotLight.diffuse"), lightDiffuse.x*isFlashlightOn, lightDiffuse.y*isFlashlightOn, lightDiffuse.z*isFlashlightOn);
		glUniform3f(cubeShader->Uni("spotLight.specular"), lightSpecular.x*isFlashlightOn, lightSpecular.y*isFlashlightOn, lightSpecular.z*isFlashlightOn);
		glUniform1f(cubeShader->Uni("spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(cubeShader->Uni("spotLight.fadeOff"), glm::cos(glm::radians(17.5f)));
		glUniform1f(cubeShader->Uni("spotLight.constant"), 1.0f);
		glUniform1f(cubeShader->Uni("spotLight.linear"), 0.007f);
		glUniform1f(cubeShader->Uni("spotLight.quadratic"), 0.0002f);

		glUniformMatrix4fv(cubeShader->Uni("proj"), 1, GL_FALSE, glm::value_ptr(proj));
		glUniformMatrix4fv(cubeShader->Uni("view"), 1, GL_FALSE, glm::value_ptr(view));

		glBindVertexArray(VAO[0]);

		for (GLuint i = 0; i <obj::NR_RECT_IN_CIRC; i++) {
			model = glm::mat4();
			model = glm::translate(model, obj::rectInCircPositions[i]);
			model = glm::rotate(model, 
                obj::rectInCircRotation[i].w, 
                glm::vec3(obj::rectInCircRotation[i].x, 
                    obj::rectInCircRotation[i].y, 
                    obj::rectInCircRotation[i].z));
			model = glm::scale(model, glm::vec3(obj::rectInCircScale[i]));
			glUniformMatrix4fv(cubeShader->Uni("model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(cubeShader->Uni("normalMat"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(view * model))));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		for (GLuint i = 0; i < obj::NR_RECT; i++) {
			model = glm::mat4();
			model = glm::translate(model, obj::rectPositions[i]);
			model = glm::scale(model, glm::vec3(obj::rectScale[i][0], 1.0f , obj::rectScale[i][1]));
			glUniformMatrix4fv(cubeShader->Uni("model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(cubeShader->Uni("normalMat"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(view * model))));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		for (GLuint i = 0; i < obj::NR_AGENT_TO_DRAW; i++) {
			model = glm::mat4();
			model = glm::translate(model, obj::agentPositions[i]);
			model = glm::rotate(model, 
                obj::agentRotation[i].w, 
                glm::vec3(obj::agentRotation[i].x, 
                    obj::agentRotation[i].y,
                    obj::agentRotation[i].z));
			model = glm::scale(model, glm::vec3(obj::agentScale[i]));
			glUniformMatrix4fv(cubeShader->Uni("model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(cubeShader->Uni("normalMat"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(view * model))));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		switch (cur_mode) {
		case 0:
			if (cur_ob != nullptr) {
                Circ * cur_cob = dynamic_cast<Circ *>(cur_ob);

				for (int i = 0; i < cylinder_res; i++) {
					model = glm::mat4();
					model = glm::translate(model, glm::vec3(cur_cob->o.x, 0.0f - 0.001f*i, cur_cob->o.y));
					model = glm::scale(model, glm::vec3(cur_cob->r * static_cast<float>(sqrt(2))));
					model = glm::rotate(model, glm::radians(360.0f / cylinder_res*i), glm::vec3(0.f, 1.f, 0.f));
					glUniformMatrix4fv(cubeShader->Uni("model"), 1, GL_FALSE, glm::value_ptr(model));
					glUniformMatrix4fv(cubeShader->Uni("normalMat"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(view * model))));

					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
			}
			break;
		case 1:
			if (cur_ob != nullptr) {
                Rect * cur_rob = dynamic_cast<Rect *>(cur_ob);

				model = glm::mat4();
				model = glm::translate(model, glm::vec3(cur_rob->o.x, 0.0f, cur_rob->o.y));
				model = glm::scale(model, glm::vec3(cur_rob->w, 1.0f, cur_rob->h));
				glUniformMatrix4fv(cubeShader->Uni("model"), 1, GL_FALSE, glm::value_ptr(model));
				glUniformMatrix4fv(cubeShader->Uni("normalMat"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(view * model))));

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			break;
		}
		/* end cube shaders */

		flatShader->use();
		glUniform3f(flatShader->Uni("material.ambient"), 1.0f, 0.5f, 0.31f);
		glUniform1f(flatShader->Uni("material.shine"), 32.0f);

		glUniform3f(flatShader->Uni("dirLight.direction"), dirLightDir.x, dirLightDir.y, dirLightDir.z);
		glUniform3f(flatShader->Uni("dirLight.ambient"), lightAmbient.x*0.1f, lightAmbient.y*0.1f, lightAmbient.z*0.1f);
		glUniform3f(flatShader->Uni("dirLight.diffuse"), lightDiffuse.x*0.1f, lightDiffuse.y*0.1f, lightDiffuse.z*0.1f);
		glUniform3f(flatShader->Uni("dirLight.specular"), lightSpecular.x*0.1f, lightSpecular.y*0.1f, lightSpecular.z*0.1f);

		for (GLuint i = 0; i < 4; i++) {
			std::string si = "pointLights[" + std::to_string(i) + "].";
			glUniform3f(flatShader->Uni( si + "position"), pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
			glUniform1f(flatShader->Uni( si + "constant"), 1.0f);
			glUniform1f(flatShader->Uni( si + "linear"), 0.045f);
			glUniform1f(flatShader->Uni( si + "quadratic"), 0.0075f);
			glUniform3f(flatShader->Uni( si + "ambient"), lightAmbient.x, lightAmbient.y, lightAmbient.z);
			glUniform3f(flatShader->Uni( si + "diffuse"), lightDiffuse.x, lightDiffuse.y, lightDiffuse.z);
			glUniform3f(flatShader->Uni( si + "specular"), lightSpecular.x, lightSpecular.y, lightSpecular.z);
		}

		glUniform3f(flatShader->Uni( "spotLight.position"), cam->pos.x, cam->pos.y, cam->pos.z);
		glUniform3f(flatShader->Uni( "spotLight.direction"), cam->dir.x, cam->dir.y, cam->dir.z);
		glUniform3f(flatShader->Uni( "spotLight.ambient"), lightAmbient.x*isFlashlightOn, lightAmbient.y*isFlashlightOn, lightAmbient.z*isFlashlightOn);
		glUniform3f(flatShader->Uni( "spotLight.diffuse"), lightDiffuse.x*isFlashlightOn, lightDiffuse.y*isFlashlightOn, lightDiffuse.z*isFlashlightOn);
		glUniform3f(flatShader->Uni( "spotLight.specular"), lightSpecular.x*isFlashlightOn, lightSpecular.y*isFlashlightOn, lightSpecular.z*isFlashlightOn);
		glUniform1f(flatShader->Uni( "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(flatShader->Uni( "spotLight.fadeOff"), glm::cos(glm::radians(17.5f)));
		glUniform1f(flatShader->Uni( "spotLight.constant"), 1.0f);
		glUniform1f(flatShader->Uni( "spotLight.linear"), 0.045f);
		glUniform1f(flatShader->Uni( "spotLight.quadratic"), 0.0075f);

		glUniformMatrix4fv(flatShader->Uni( "proj"), 1, GL_FALSE, glm::value_ptr(proj));
		glUniformMatrix4fv(flatShader->Uni( "view"), 1, GL_FALSE, glm::value_ptr(view));

		for (GLuint i = 0; i < obj::NR_CUBES; i++) {
			glUniform3f(flatShader->Uni("material.diffuse"), obj::cubeDiffuseColor[i].x, obj::cubeDiffuseColor[i].y, obj::cubeDiffuseColor[i].z);
			glUniform3f(flatShader->Uni("material.specular"), obj::cubeSpecularColor[i].x, obj::cubeSpecularColor[i].y, obj::cubeSpecularColor[i].z);
			model = glm::mat4();
			model = glm::translate(model, obj::cubePositions[i]);
			model = glm::scale(model, glm::vec3(obj::cubeScale[i]));
			glUniformMatrix4fv(flatShader->Uni("model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(flatShader->Uni( "normalMat"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(view * model))));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		/* end flat shaders */

		lampShader->use();
		glUniformMatrix4fv(lampShader->Uni( "proj"), 1, GL_FALSE, glm::value_ptr(proj));
		glUniformMatrix4fv(lampShader->Uni("view"), 1, GL_FALSE, glm::value_ptr(view));

		glUniform3f(lampShader->Uni("lightColor"), lightSpecular.x, lightSpecular.y, lightSpecular.z);

		for (int i = 0; i < 4; i++) {
			model = glm::mat4();
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			glUniformMatrix4fv(lampShader->Uni("model"), 1, GL_FALSE, glm::value_ptr(model));

			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);
		/* end lamp shaders */

		// Swap the Double Buffer
		glfwSwapBuffers(window);
    }
	
	D(std::cout << std::endl << "Exiting Game Loop..." << std::endl << std::endl);
	

	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);

	/* Exit */
    return DIE(EXIT_SUCCESS);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    UNUSED(window, scancode, mode);

	//Press ESC to close Application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (action == GLFW_PRESS) {
		D(std::cout << "Key Pressed: " << /*glfwGetKeyName(key, key)*/ key << std::endl);
		keys[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		keys[key] = false;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    UNUSED(window);
	if (!mouse::focus) {
		mouse::lastX = (GLfloat) xpos;
		mouse::lastY = (GLfloat) ypos;
		mouse::focus = true;
	}

	GLfloat xoffset = (GLfloat) xpos - mouse::lastX;
	GLfloat yoffset = (GLfloat) ypos - mouse::lastY;
	mouse::lastX = (GLfloat) xpos;
	mouse::lastY = (GLfloat) ypos;
	//xoffset *= mouse::sensitivity;
	//yoffset *= mouse::sensitivity;

	cam->mouseRotateCamera(xoffset, yoffset);
}

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset) {
    UNUSED(window, xoffset);

	cam->scrollZoomCamera((GLfloat) yoffset);
}

void do_movement() {
	if (keys[GLFW_KEY_W])
		cam->translateCamera(G::CAMERA::FORWARD, timer->getDelta());
	if (keys[GLFW_KEY_S])
		cam->translateCamera(G::CAMERA::BACKWARD, timer->getDelta());
	if (keys[GLFW_KEY_A])
		cam->translateCamera(G::CAMERA::LEFT, timer->getDelta());
	if (keys[GLFW_KEY_D])
		cam->translateCamera(G::CAMERA::RIGHT, timer->getDelta());

	if (keys[GLFW_KEY_P]) {
		keys[GLFW_KEY_P] = false;
		replan();
		init_planning_vis();
	}

    //deprecated for the uniform grid to work
	if (!G::WITH_TTC_GRID && keys[GLFW_KEY_G]) {
		keys[GLFW_KEY_G] = false;
        selected_agent_debug = (selected_agent_debug + 1) % NUM_AGENTS;
        init_planning_vis();
	}
	if (!G::WITH_TTC_GRID && keys[GLFW_KEY_T]) {
		keys[GLFW_KEY_T] = false;
        selected_agent_debug = (selected_agent_debug - 1) % NUM_AGENTS;
        init_planning_vis();
	}
    

    /*///the dynamic obstacle is buggy AF with multi-agents for some reason
    if (keys[GLFW_KEY_O]) {
    keys[GLFW_KEY_O] = false;
    placeObst(cam->pos);
    }
	if (keys[GLFW_KEY_M]) {
		keys[GLFW_KEY_M] = false;
		modeToggleCurrentObstacle();
	}
	if (keys[GLFW_KEY_I])
		scaleCurrentObstacle(1.f, 1.1f, timer->getDelta());
	if (keys[GLFW_KEY_K])
		scaleCurrentObstacle(1.f, 1/1.1f, timer->getDelta());
	if (keys[GLFW_KEY_J])
		scaleCurrentObstacle(1 / 1.1f, 1.f, timer->getDelta());
	if (keys[GLFW_KEY_L])
		scaleCurrentObstacle(1.1f, 1.f, timer->getDelta());
    */
	if (keys[GLFW_KEY_UP])
		movePlayer( 0.f,  1.3f, timer->getDelta());
	if (keys[GLFW_KEY_DOWN])
		movePlayer( 0.f, -1.3f, timer->getDelta());
	if (keys[GLFW_KEY_LEFT])
		movePlayer(-1.3f,  0.f, timer->getDelta());
	if (keys[GLFW_KEY_RIGHT])
		movePlayer( 1.3f,  0.f, timer->getDelta());

	if (keys[GLFW_KEY_F]) {
		keys[GLFW_KEY_F] = false;
		toggleFlashlight();
	}
}

int DIE(int retVal) {
	glfwTerminate();

	std::cerr << std::endl << "Application Terminated. With exit value: " << retVal << std::endl;
	D(slowPrint(50, 300, "\n\nGoodbye..."));
	D(slowPrint(150, 500, "OK"));

	return retVal;
}

void lookahead(glm::vec2 * agentNow, glm::vec2 * nextNode, Agent * a, float * speed, float dt) {
    if (a->completed_nodes < a->plan->size()) {
        *nextNode = (*a->plan)[a->completed_nodes]->data;

        while (a->completed_nodes + 1 < a->plan->size()
            && a->cspace->lineOfSight(*agentNow, (*a->plan)[a->completed_nodes + 1]->data)) {
            a->completed_nodes++;
            *nextNode = (*a->plan)[a->completed_nodes]->data;
        }

        float dist = glm::distance(*nextNode, *agentNow);
        if (dist < 0.1f) {
            a->completed_nodes++;
            //not sure why I have this, as this will, like, never happen.
            if (dist < dt*(*speed)) {//detects overshooting
                *agentNow = *nextNode;//move to the node, but not past...
                *speed -= dist / dt;
            }
        }
    }
    else
        *nextNode = (*a->plan)[a->plan->size() - 1]->data;
}

void ttc_forces_(double ttc, glm::vec2 *FAVOID, glm::vec2 dir) {
    dir /= glm::length(dir);

    double t_h = 3.0;//seconds
    double mag = 0;
    if (ttc >= 0 && ttc <= t_h)
        mag = (t_h - ttc) / (ttc + 0.001);
    mag = mag > 20 ? 20 : mag;
    *FAVOID += glm::vec2(mag * dir.x, mag * dir.y);
}
void ttc_forces(Agent * a, Circ * b, double ttc, glm::vec2 * FAVOID) {
    glm::vec2 V_dt(a->vel.x * ttc, a->vel.y * ttc);
    glm::vec2 dir = (a->bv->o + V_dt - b->o);
    ttc_forces_(ttc, FAVOID, dir);
}
void ttc_forces(Agent * a, Agent * b, double ttc, glm::vec2 * FAVOID) {
    glm::vec2 V_dt(a->vel.x * ttc, a->vel.y * ttc);
    glm::vec2 bV_dt(b->vel.x * ttc, b->vel.y * ttc);
    glm::vec2 dir = (a->bv->o + V_dt - b->bv->o - bV_dt);
    ttc_forces_(ttc, FAVOID, dir);
}

void boid_forces(Agent * a, glm::vec2 * FBOID) {
    const float boid_speed = 1.2f;

    glm::vec2 avg_vel(0, 0), avg_pos(0, 0), avg_dir(0, 0);
    GLfloat cohes_r_look = 1.0f, align_r_look = 1.0f, separ_r_look = .5f;//limit to search for forces for boidlings
    GLfloat ff0_r = 2.0f;//radius of following force of 0
    GLfloat ff1_r = 10.0f;//radius of following force of 1 towards leader
    glm::vec2 FALIGN, FCOHES, FFOLOW, FSEPAR;

    for (int i = 0; i < boidlings.size(); i++) {
        Agent * boid = boidlings[i];
        if (boid == a)
            continue;
        glm::vec2 dist = boid->bv->o - a->bv->o;
        if (glm::dot(dist, dist) < align_r_look * align_r_look)
            avg_vel = (avg_vel * static_cast<float>(i) + boid->vel) / static_cast<float>(i + 1);
        if (glm::dot(dist, dist) < cohes_r_look * cohes_r_look)
            avg_pos = (avg_pos * static_cast<float>(i) + boid->bv->o) / static_cast<float>(i + 1);
    }

    /* alignnment force */
    //average velocity; pull towards that
    float norm = glm::length(avg_vel);
    if (norm != 0)
        avg_vel /= norm;
    FALIGN = (avg_vel - a->vel) * boid_speed;

    /* cohesion force */
    //average cohesion; pull towards that
    FCOHES = avg_pos - a->bv->o;

    /* follow force */
    //distance to leader, weak close, strong far
    glm::vec2 toLeader = player->o - a->bv->o;
    float dist2 = glm::dot(toLeader, toLeader);
    if (dist2 < ff0_r * ff0_r)
        FFOLOW = glm::vec2(0);
    else
        FFOLOW = toLeader * (dist2 - ff0_r) / (ff1_r - ff0_r);

    /* separation force */
    //force from inverted direction of nearest neighbours
    for (int i = 0; i < boidlings.size(); i++) {
        Agent * boid = boidlings[i];
        if (boid == a)
            continue;
        glm::vec2 toBoid = boid->bv->o - a->bv->o;
        float dist2 = glm::dot(toBoid, toBoid);

        if (dist2 < separ_r_look * separ_r_look)
            FSEPAR += -toBoid / (10*sqrt(dist2));
    }

    (*FBOID) += FALIGN + FCOHES + FFOLOW+ FSEPAR;
    if (glm::dot(*FBOID, *FBOID) > 20 * 20) {
        *FBOID /= glm::length(*FBOID);
        *FBOID *= 20.f;
    }
}

void force_agents(GLfloat dt, Agent * a, int * i, int * j, std::vector<Agent *> newGrid[100][100]) {
    
    float speed = 1.0f; // x m/s
    glm::vec2 agentNow = a->bv->o;
    glm::vec2 nextNode;
    glm::vec2 goalV;
    if (a->plan != nullptr && 0 < a->plan->size()) {
        lookahead(&agentNow, &nextNode, a, &speed, dt);
        goalV = (nextNode - agentNow) / glm::distance(nextNode, agentNow) * (speed /* * dt */);
    }
    else {
        nextNode = agentNow;
        goalV = glm::vec2(0);
    }

    /* ttc - approximate */
    glm::vec2 goalF;
    if (a->boid)
        goalF = glm::vec2(0);
    else
        goalF = 2.0f*(goalV - a->vel);

    glm::vec2 FSUM = goalF;

    glm::vec2 FAVOID(0), FBOID(0);

    if (G::WITH_TTC_GRID) {
        for (int k = (0 < *i - 4) ? *i - 4 : 0; k < ((100 > *i + 4) ? *i + 4 : 100); k++) { // spatial search
            for (int l = (0 < *j - 4) ? *j - 4 : 0; l < ((100 > *j + 4) ? *j + 4 : 100); l++) {
                for (Agent * b : agents[k][l]) {
                    if (a == b)
                        continue;

                    double ttc = LMP::ttc(a->bv, a->vel, b->bv, b->vel);

                    if (ttc > 10)
                        continue;

                    //suggestion to handle collisions
                    //float originalAr = static_cast<Circ *>(a->bv)->r;
                    //float originalBr = static_cast<Circ *>(b->bv)->r;
                    ttc_forces(a, b, ttc, &FAVOID);
                }
            }
        }
    }//with ttc grid
    else {
        for (Agent * b : agents_old) {
            if (a == b)
                continue;

            double ttc = LMP::ttc(a->bv, a->vel, b->bv, b->vel);

            if (ttc > 10)
                continue;

            ttc_forces(a, b, ttc, &FAVOID);
        }
    }

    for (Circ * c : obstBounds) {
        double ttc = LMP::ttc(a->bv, a->vel, c, glm::vec2(0));

        if (ttc > 10)
            continue;

        ttc_forces(a, c, ttc, &FAVOID);
    }

    if (a->boid) {
        boid_forces(a, &FBOID);
    }

    FSUM += FAVOID + FBOID;
    a->vel += FSUM * dt;
    a->bv->o += a->vel * dt;

    if (G::WITH_TTC_GRID) {
        int x = static_cast<int>(a->bv->o.x * 5 + 50);
        if (x < 0) x = 0;
        if (x > 99) x = 99;
        int y = static_cast<int>(a->bv->o.y * 5 + 50);
        if (y < 0) y = 0;
        if (y > 99) y = 99;
        (newGrid)[x][y].push_back(a);
    }
}

void animate_agents(GLfloat dt) {
    std::vector<Agent *> newGrid[100][100];
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            newGrid[i][j] = std::vector<Agent *>();
        }
    }

    if (G::WITH_TTC_GRID)
        for (int i = 0; i < 100; i++)
            for (int j = 0; j < 100; j++)
                for (Agent * a : agents[i][j])
                    force_agents(dt, a, &i, &j, newGrid);
    else
        for (Agent * a : agents_old)
            force_agents(dt, a, nullptr, nullptr, nullptr);

    if (G::WITH_TTC_GRID)
        for (int i = 0; i < 100; i++)
            for (int j = 0; j < 100; j++)
                agents[i][j] = newGrid[i][j];

    int agent_mesh_drawn = 0;
    if (G::WITH_TTC_GRID) { //EVERY SINGLE ONE OF THESE COULD HAVE THEIR INTERNALS AS FUNCTIONS
        for (int i = 0; i < 100; i++) {
            for (int j = 0; j < 100; j++) {
                for (Agent * a : agents[i][j]) {
                    int step;
                    if (a->vt == agent::volume_type::RECT)
                        step = 1;
                    else
                        step = cylinder_res;

                    for (GLuint i = 0; i < step; i++)
                        obj::agentPositions[i + agent_mesh_drawn] = glm::vec3(a->bv->o.x, 0.0f + 0.001f*(i /*+ agent_mesh_drawn*/), a->bv->o.y);
                    agent_mesh_drawn += step;
                }
            }
        }
    }
    else {
        for (Agent * a : agents_old) {
            int step;
            if (a->vt == agent::volume_type::RECT)
                step = 1;
            else
                step = cylinder_res;

            for (GLuint i = 0; i < step; i++)
                obj::agentPositions[i + agent_mesh_drawn] = glm::vec3(a->bv->o.x, 0.0f + 0.001f*(i /*+ agent_mesh_drawn*/), a->bv->o.y);
            agent_mesh_drawn += step;
        }
    }
}

void init_planning() {
	cur_ob = nullptr;
    switch (G::SCENARIO) {
    case G::SCENE::DEFAULT:
    case G::SCENE::DEADEND:
    case G::SCENE::NO_BOID:
    case G::SCENE::MAZE:
        player = new Circ(glm::vec2(0.f), .5f);
        break;
    case G::SCENE::WALL:
        player = new Circ(glm::vec2(5.f, 0.f), .5f);
        break;
    }

    double boidlingSize;
    glm::vec2 boidspawn, boidlingRanks, boidlingOffset, boidlingSpacing;
    double agentSize;
    glm::vec2 spacing, groupSize, offset, start1, start2, goal1, goal2;

    switch (G::SCENARIO) {
    case G::SCENE::DEFAULT:
    case G::SCENE::DEADEND:
    case G::SCENE::MAZE:
        /* PLAY WITH THESE */
        agentSize = .05;
        spacing = glm::vec2(.25, .25);
        groupSize = glm::vec2(21, 21);
        offset = glm::vec2(21. / 2. - .5, 21. / 2. - .5);
        start1 = glm::vec2(-7., -7.);
        start2 = glm::vec2(7., 7.);
        goal1 = glm::vec2(7., 7.);
        goal2 = glm::vec2(-7., -7.);

        boidlingSize = .05;
        boidspawn = glm::vec2(-2.0f, 0.0f);
        boidlingRanks = glm::vec2(9, 9);
        boidlingOffset = glm::vec2(5. / 2. - .5, 5. / 2. - .5);
        boidlingSpacing = glm::vec2(.3f, .3f);
        /* */
        break;
    case G::SCENE::WALL:
        agentSize = .05;
        spacing = glm::vec2(.25, .25);
        groupSize = glm::vec2(21, 21);
        offset = glm::vec2(21. / 2. - .5, 21. / 2. - .5);
        start1 = glm::vec2(-7., -7.);
        start2 = glm::vec2(7., 7.);
        goal1 = glm::vec2(7., 7.);
        goal2 = glm::vec2(-7., -7.);

        boidlingSize = .05;
        boidspawn = glm::vec2(6.0f, 0.0f);
        boidlingRanks = glm::vec2(9, 9);
        boidlingOffset = glm::vec2(5. / 2. - .5, 5. / 2. - .5);
        boidlingSpacing = glm::vec2(.3f, .3f);
        break;
    case G::SCENE::NO_BOID:
        agentSize = .05;
        spacing = glm::vec2(.25, .25);
        groupSize = glm::vec2(21, 21);
        offset = glm::vec2(21. / 2. - .5, 21. / 2. - .5);
        start1 = glm::vec2(-7., -7.);
        start2 = glm::vec2(-7., 7.);
        goal1 = glm::vec2(7., 7.);
        goal2 = glm::vec2(7., -7.);

        boidlingRanks = glm::vec2(0, 0);
        break;
    }

    if (G::WITH_TTC_GRID)
        for (int i = 0; i < 100; i++)
            for (int j = 0; j < 100; j++)
                agents[i][j] = std::vector<Agent *>();
    else
        agents_old = std::vector<Agent *>(groupSize.x * groupSize.y * 2 + boidlingRanks.x * boidlingRanks.y);
    //only use circs since I did not expand ttc for my rects yet

    for (int i = 0; i < groupSize.x; i++) {
        for (int j = 0; j < groupSize.y; j++) {
            glm::vec2 o(start1.x + spacing.x * (i - offset.x), start1.y + spacing.y * (j - offset.y));
            Agent * a = new Agent(agent::volume_type::CIRC,
                new Circ(o, agentSize),
                glm::vec2(
                    goal1.x + spacing.y*(i - offset.x),
                    goal1.y + spacing.y*(j - offset.y)));
            if (G::WITH_TTC_GRID) {
                int x = static_cast<int>(o.x * 5 + 50);
                int y = static_cast<int>(o.y * 5 + 50);

                agents[x][y].push_back(a);
                NUM_AGENTS++;
            }
            else {
                agents_old[i + groupSize.x * j] = a;
            }
        }
    }

    for (int i = 0; i < groupSize.x; i++) {
        for (int j = 0; j < groupSize.y; j++) {
            glm::vec2 o(start2.x + spacing.x * (i - offset.x), start2.y + spacing.y * (j - offset.y));
            Agent * a = new Agent(agent::volume_type::CIRC,
                new Circ(o, agentSize),
                glm::vec2(
                    goal2.x + spacing.x*(i - offset.x),
                    goal2.y + spacing.y*(j - offset.y)));
            if (G::WITH_TTC_GRID) {
                int x = static_cast<int>(o.x * 5 + 50);
                int y = static_cast<int>(o.y * 5 + 50);

                agents[x][y].push_back(a);
                NUM_AGENTS++;
            }
            else {
                agents_old[i + groupSize.x * j + groupSize.x * groupSize.y] = a;
            }
        }
    }

    for (int i = 0; i < boidlingRanks.x; i++) {
        for (int j = 0; j < boidlingRanks.y; j++) {
            glm::vec2 o(boidspawn.x + boidlingSpacing.x * (i - boidlingOffset.x), boidspawn.y + boidlingSpacing.y * (j - boidlingOffset.y));
            Agent * a = new Agent(agent::volume_type::CIRC, new Circ(o, boidlingSize), glm::vec2(0));
            a->boid = true;

            if (G::WITH_TTC_GRID) {
                int x = static_cast<int>(o.x * 5 + 50);
                int y = static_cast<int>(o.y * 5 + 50);

                agents[x][y].push_back(a);
                NUM_AGENTS++;
            }
            else {
                agents_old[i + groupSize.x * j + 2 * groupSize.x * groupSize.y] = a;
            }

            boidlings.push_back(a);
        }
    }
    
    switch (G::SCENARIO) {
    case G::SCENE::DEFAULT:
    case G::SCENE::NO_BOID:
        obstBounds = std::vector<Circ *>(2);
        obstBounds[0] = new Circ(glm::vec2(-3.0f, -3.0f), 2.0f);
        obstBounds[1] = new Circ(glm::vec2(-2.0f, 6.0f), 1.0f);
        rectBounds = std::vector<Rect *>();
        obstBounds.push_back(player);
        break;
    case G::SCENE::DEADEND:
        obstBounds = std::vector<Circ *>(9);
        for (int i = 0; i < 5; i++)
            obstBounds[i] = new Circ(glm::vec2(-3.5f, i - 7.5), .49f);
        for (int i = 0; i < 4; i++)
            obstBounds[i + 5] = new Circ(glm::vec2(i - 7.5, -3.5f), .49f);

        rectBounds = std::vector<Rect *>();
        obstBounds.push_back(player);
        break;
    case G::SCENE::WALL:
        obstBounds = std::vector<Circ *>(36);
        for (int sign = -1; sign < 2; sign += 2)
            for (float i = 0; i < 18; i++)
                obstBounds[i + ((1+sign)/2)*18 ] = new Circ(glm::vec2(-sign * (i / 2.f + 1.f), sign * (i / 2.f + 1.f)), .5f);
        rectBounds = std::vector<Rect *>();
        obstBounds.push_back(player);
        break;
    case G::SCENE::MAZE:
        obstBounds = std::vector<Circ *>(2);
        obstBounds[0] = new Circ(glm::vec2(-3.0f, -3.0f), 2.0f);
        obstBounds[1] = new Circ(glm::vec2(-2.0f, 6.0f), 1.0f);
        rectBounds = std::vector<Rect *>(10);
        {int NR = 0;
        //fix these at some point..
        rectBounds[NR] = new Rect(glm::vec2(3.0f, 8.0f), 0.1f, 4.0f); NR++;
        rectBounds[NR] = new Rect(glm::vec2(-5.0f, -1.0f), 5.5f, 0.1f); NR++;
        rectBounds[NR] = new Rect(glm::vec2(3.0f, -7.0f), 0.1f, 6.0f); NR++;
        rectBounds[NR] = new Rect(glm::vec2(3.0f, 0.0f), 0.1f, 4.0f); NR++;
        rectBounds[NR] = new Rect(glm::vec2(-6.5f, -2.0f), 0.1f, 8.0f); NR++;//
        rectBounds[NR] = new Rect(glm::vec2(6.0f, 2.0f), 6.0f, 0.1f); NR++;
        rectBounds[NR] = new Rect(glm::vec2(-6.0f, 6.0f), 3.0f, 0.1f); NR++;
        rectBounds[NR] = new Rect(glm::vec2(-6.0f, 6.0f), 0.1f, 3.0f); NR++;
        rectBounds[NR] = new Rect(glm::vec2(0.0f, 4.0f), 0.1f, 4.0f); NR++;
        rectBounds[NR] = new Rect(glm::vec2(-3.0f,    6.0f),    6.0f,   0.1f); NR++;//
        }
        obstBounds.push_back(player);
        break;
    }
}

void init_planning_vis() {
    Agent * a;
    if (!G::WITH_TTC_GRID)
        a = agents_old[selected_agent_debug];

    //DEPRECATED for the sake of the uniform grid working :P
    if (!G::WITH_TTC_GRID && a->prm != nullptr) {
        std::vector<Node<glm::vec2> *> * verts = a->prm->roadmap->vertices;
        obj::NR_CUBES = static_cast<GLuint>(verts->size() + 1);
        obj::cubePositions = new glm::vec3[obj::NR_CUBES];
        obj::cubeScale = new float[obj::NR_CUBES];
        obj::cubeDiffuseColor = new glm::vec3[obj::NR_CUBES];
        obj::cubeSpecularColor = new glm::vec3[obj::NR_CUBES];

        for (GLuint i = 0; i < obj::NR_CUBES - 1; i++) {
            Node<glm::vec2> * v = verts->at(i);
            obj::cubePositions[i] = glm::vec3(v->data.x, -2.0f, v->data.y);
            if (i == 0) {//start = blue
                obj::cubeScale[i] = 1.0f;
                obj::cubeDiffuseColor[i] = glm::vec3(0.0f, 0.0f, 1.0f);
                obj::cubeSpecularColor[i] = glm::vec3(1.0f, 1.0f, 1.0f);
                obj::cubePositions[i] = glm::vec3(a->start.x, -2.0f, a->start.y);
            }
            else if (i == 1) {//goal = red
                obj::cubeScale[i] = 1.0f;
                obj::cubeDiffuseColor[i] = glm::vec3(1.0f, 0.0f, 0.0f);
                obj::cubeSpecularColor[i] = glm::vec3(1.0f, 1.0f, 1.0f);
                obj::cubePositions[i] = glm::vec3(a->goal.x, -2.0f, a->goal.y);
            }
            else if (find(a->plan->begin(), a->plan->end(), v) != a->plan->end()) {
                obj::cubeScale[i] = 0.75f;
                obj::cubeDiffuseColor[i] = glm::vec3(0.0f, 1.0f, 0.0f);
                obj::cubeSpecularColor[i] = glm::vec3(1.0f, 1.0f, 1.0f);
            }
            else {
                obj::cubeScale[i] = 0.5f;
                obj::cubeDiffuseColor[i] = glm::vec3(0.4f, 0.4f, 0.4f);
                obj::cubeSpecularColor[i] = glm::vec3(1.0f, 1.0f, 1.0f);
            }
        }

        //std::vector<std::pair<Node<glm::vec2> *, Node<glm::vec2> *>> * all_edges = prm->roadmap->all_edges;
        //glBufferData(,,)
    }
    else {
        obj::NR_CUBES = 1;
        obj::cubePositions = new glm::vec3[obj::NR_CUBES];
        obj::cubeScale = new float[obj::NR_CUBES];
        obj::cubeDiffuseColor = new glm::vec3[obj::NR_CUBES];
        obj::cubeSpecularColor = new glm::vec3[obj::NR_CUBES];
    }

    // add ground
    obj::cubeScale[obj::NR_CUBES - 1] = 20.0f;
    obj::cubePositions[obj::NR_CUBES - 1] = glm::vec3(0.0f, -12.0f, 0.0f);
    obj::cubeDiffuseColor[obj::NR_CUBES - 1] = glm::vec3(0.3f, 0.5f, 0.3f);
    obj::cubeSpecularColor[obj::NR_CUBES - 1] = glm::vec3(1.0f, 1.0f, 1.0f);


    // obstacles -- could be polymorphised :p
    //add circs
    obj::NR_RECT_IN_CIRC = static_cast<GLuint>(cylinder_res * obstBounds.size());
    obj::rectInCircPositions = new glm::vec3[obj::NR_RECT_IN_CIRC];
    obj::rectInCircRotation = new glm::vec4[obj::NR_RECT_IN_CIRC];
    obj::rectInCircScale = new float[obj::NR_RECT_IN_CIRC];
    for (GLuint i = 0; i < obj::NR_RECT_IN_CIRC; i++) {
        obj::rectInCircPositions[i] = glm::vec3(obstBounds[(int)i / cylinder_res]->o.x, 0.0f - 0.001f*i, obstBounds[(int)i / cylinder_res]->o.y);
        obj::rectInCircScale[i] = obstBounds[(int)i / cylinder_res]->r * static_cast<float>(sqrt(2));
        obj::rectInCircRotation[i] = glm::vec4(0.0f, 1.0f, 0.0f, glm::radians(360.0f / cylinder_res * (i%cylinder_res)));
    }

    //add rects
    obj::NR_RECT = static_cast<GLuint>(rectBounds.size());
    obj::rectPositions = new glm::vec3[obj::NR_RECT];
    obj::rectScale = new glm::vec2[obj::NR_RECT];
    for (GLuint i = 0; i < obj::NR_RECT; i++) {
        obj::rectPositions[i] = glm::vec3(rectBounds[i]->o.x, 0.0f, rectBounds[i]->o.y);
        obj::rectScale[i] = glm::vec2(rectBounds[i]->w, rectBounds[i]->h);
    }

    //add agent -- could be polymorphised -- :P
    obj::NR_AGENT_TO_DRAW = 0;
    if (G::WITH_TTC_GRID) {//todo  internals -> function
        for (int i = 0; i < 100; i++)
            for (int j = 0; j < 100; j++)
                for (Agent * a : agents[i][j])
                    if (a->vt == agent::volume_type::RECT)
                        obj::NR_AGENT_TO_DRAW++;
                    else
                        obj::NR_AGENT_TO_DRAW += cylinder_res;
    }
    else {
        for (Agent * a : agents_old)
            if (a->vt == agent::volume_type::RECT)
                obj::NR_AGENT_TO_DRAW++;
            else
                obj::NR_AGENT_TO_DRAW += cylinder_res;
    }
    

    obj::agentPositions = new glm::vec3[obj::NR_AGENT_TO_DRAW]; 
    obj::agentScale = new GLfloat[obj::NR_AGENT_TO_DRAW]; 
    obj::agentRotation = new glm::vec4[obj::NR_AGENT_TO_DRAW];

    int agent_mesh_drawn = 0;
    if (G::WITH_TTC_GRID) {//todo internals -> function / macro / preprocess
        for (int i = 0; i < 100; i++)
            for (int j = 0; j < 100; j++)
                for (Agent * a : agents[i][j]) {
                    int step;
                    if (a->vt == agent::volume_type::RECT)
                        step = 1;
                    else
                        step = cylinder_res;

                    for (GLuint i = 0; i < step; i++) {
                        if (a->vt == agent::volume_type::CIRC) {
                            obj::agentPositions[i + agent_mesh_drawn] =
                                glm::vec3(a->bv->o.x, 0.0f + 0.001f*i, a->bv->o.y);
                            obj::agentScale[i + agent_mesh_drawn] =
                                static_cast<Circ *>(a->bv)->r * static_cast<float>(sqrt(2));
                            obj::agentRotation[i + agent_mesh_drawn] =
                                glm::vec4(0.0f, 1.0f, 0.0f, glm::radians(360.0f / cylinder_res * (i%cylinder_res)));
                        }
                        else {
                            obj::agentPositions[i + agent_mesh_drawn] =
                                glm::vec3(a->bv->o.x, 0.0f, a->bv->o.y);
                            obj::agentScale[i + agent_mesh_drawn] =
                                static_cast<Rect *>(a->bv)->w;
                            obj::agentRotation[i + agent_mesh_drawn] =
                                glm::vec4(0.0f, 1.0f, 0.0f, 0);
                        }

                    }

                    agent_mesh_drawn += step;
                }
    }
    else {
        for (Agent * a : agents_old) {
            int step;
            if (a->vt == agent::volume_type::RECT)
                step = 1;
            else
                step = cylinder_res;

            for (GLuint i = 0; i < step; i++) {
                if (a->vt == agent::volume_type::CIRC) {
                    obj::agentPositions[i + agent_mesh_drawn] =
                        glm::vec3(a->bv->o.x, 0.0f + 0.001f*i, a->bv->o.y);
                    obj::agentScale[i + agent_mesh_drawn] =
                        static_cast<Circ *>(a->bv)->r * static_cast<float>(sqrt(2));
                    obj::agentRotation[i + agent_mesh_drawn] =
                        glm::vec4(0.0f, 1.0f, 0.0f, glm::radians(360.0f / cylinder_res * (i%cylinder_res)));
                }
                else {
                    obj::agentPositions[i + agent_mesh_drawn] =
                        glm::vec3(a->bv->o.x, 0.0f, a->bv->o.y);
                    obj::agentScale[i + agent_mesh_drawn] =
                        static_cast<Rect *>(a->bv)->w;
                    obj::agentRotation[i + agent_mesh_drawn] =
                        glm::vec4(0.0f, 1.0f, 0.0f, 0);
                }

            }

            agent_mesh_drawn += step;
        }
    }    
}


void replan() {
    timer->stopTimer();
	switch (cur_mode) {
	case 0:
		if (cur_ob != nullptr) {
            Circ * cur_cob = dynamic_cast<Circ *>(cur_ob);
			obstBounds.push_back(cur_cob);
			delete cur_cob;
			cur_cob = nullptr;
		}
		break;
	case 1:
		if (cur_ob != nullptr) {
            Rect * cur_rob = dynamic_cast<Rect *>(cur_ob);
			rectBounds.push_back(cur_rob);
			delete cur_rob;
			cur_rob = nullptr;
		}
		break;
	}

    //int positions_updated = 0;

    if (G::WITH_TTC_GRID)
        for (int i = 0; i < 100; i++)
            for (int j = 0; j < 100; j++)
                for (Agent * a : agents[i][j]) {
                    if (!a->boid) {
                        a->bv->o = a->start;
                        a->vel = glm::vec2(0, 0);
                    }

                    //int step;
                    //if (a->vt == agent::volume_type::RECT)
                    //    step = 1;
                    //else
                    //    step = cylinder_res;

                    //for (GLuint i = 0; i < step; i++)
                    //    obj::agentPositions[i + positions_updated] = glm::vec3(a->start.x, 0.0f, a->start.y);

                    //positions_updated += step;
                }
    else {
        for (Agent * a : agents_old) {
            if (!a->boid) {
                a->bv->o = a->start;
                a->vel = glm::vec2(0, 0);
            }
        }
    }



    std::vector<BoundingVolume *> bv;
    bv.reserve(obstBounds.size() + rectBounds.size());// + 2 * NUM_AGENTS - 2);
    bv.insert(bv.end(), obstBounds.begin(), obstBounds.end());
    bv.insert(bv.end(), rectBounds.begin(), rectBounds.end());

    if (G::WITH_TTC_GRID) {
        Cspace_2D * std_cspace = nullptr;
        for (int i = 0; i < 100; i++)
            for (int j = 0; j < 100; j++)
                for (Agent * a : agents[i][j]) {
                    /*
                    std::vector<BoundingVolume *> a_cspace_bvs = bv;

                    for (Agent * b : agents) {
                        if (a == b)
                            continue;

                        /*
                        //pointers were causing some aliasing issues must fix
                        BoundingVolume * s = b->bv;
                        s->o = b->start;
                        BoundingVolume * g = b->bv;
                        g->o = b->goal;
                        /
                        BoundingVolume * s, * g;
                        if (b->vt == agent::volume_type::RECT) {
                            Rect * r = static_cast<Rect *>(b->bv);
                            s = new Rect(b->start, r->w, r->h);
                            g = new Rect(b->goal, r->w, r->h);
                        }
                        else {
                            Circ * c = static_cast<Circ *>(b->bv);
                            s = new Circ(b->start, c->r);
                            g = new Circ(b->goal, c->r);
                        }

                        //a_cspace_bvs.push_back(s);
                        //a_cspace_bvs.push_back(g);
                    }
                    */
                    //even when the agents are all the same the Cspaces must vary a bit, there's definitely a more
                    //efficient way of handling this though
                    if (!a->boid) {
                        if (std_cspace == nullptr) {
                            std_cspace = new Cspace_2D(bv/*a_cspace_bvs*/, a->bv);
                        }

                        a->cspace = std_cspace;
                        a->prm = new PRM(a->start, a->goal, a->cspace);
                    }
                }
    }
    else {
        Cspace_2D * std_cspace = new Cspace_2D(bv, agents_old[0]->bv);
        for (Agent * a : agents_old) {
            if (!a->boid) {
                a->cspace = std_cspace;
                a->prm = new PRM(a->start, a->goal, a->cspace);
            }
        }
    }


	/* PATH PLANNING METHOD */

    if (G::WITH_TTC_GRID) {
        for (int i = 0; i < 100; i++)
            for (int j = 0; j < 100; j++)
                for (Agent * a : agents[i][j]) {
                    if (!a->boid) {
                        a->plan = GMP::findPathUCS(a->prm->roadmap);
                        //path_ = new std::unordered_set<Node<glm::vec2> *>();
                        //for (int i = 0; i < pathVec->size(); i++)
                        //    path_->insert((*pathVec)[i]);

                        a->completed_nodes = 0;
                    }
                }
    }
    else {
        for (Agent * a : agents_old) {
            if (!a->boid) {
                a->plan = GMP::findPathUCS(a->prm->roadmap);
                a->completed_nodes = 0;
            }
        }
    }

    init_planning_vis();
    timer->restartTimer();
}

void toggleFlashlight() {
	isFlashlightOn = !isFlashlightOn;
}

void placeObst(glm::vec3 pos) {
	switch (cur_mode) {
	case 0:
		if (cur_ob != nullptr) {
            Circ * cur_cob = dynamic_cast<Circ *>(cur_ob);
			obstBounds.push_back(cur_cob);
			init_planning_vis();
			delete cur_cob;
		}
		cur_ob = new Circ(glm::vec2(pos.x, pos.z), 1.0f);
		break;
	case 1:
        if (cur_ob != nullptr) {
            Rect * cur_cob = dynamic_cast<Rect *>(cur_ob);
            rectBounds.push_back(cur_cob);
            init_planning_vis();
            delete cur_cob;
        }
        cur_ob = new Rect(glm::vec2(pos.x, pos.z), 1.0f, 1.0f);
        break;
	}

    //replan();
}

//deprecated
void placeGoalNode(glm::vec3 pos) {
	agents_old[0]->goal.x = pos.x; agents_old[0]->goal.y = pos.z;
	init_planning_vis();
}

//deprecated
void placeStartNode(glm::vec3 pos) {
    agents_old[0]->start.x = pos.x; agents_old[0]->start.y = pos.z;
	init_planning_vis();
}

void modeToggleCurrentObstacle() {
	switch (cur_mode) {
	case 0:
		if (cur_ob != nullptr) {
            Circ * cur_cob = dynamic_cast<Circ *>(cur_ob);
			glm::vec3 pos = glm::vec3(cur_cob->o.x, 0.0f, cur_cob->o.y);
			delete cur_cob;
			cur_cob = nullptr;
			cur_mode = 1 - cur_mode;
			placeObst(pos);
		}
		break;
	case 1:
		if (cur_ob != nullptr) {
            Rect * cur_rob = dynamic_cast<Rect *>(cur_ob);
			glm::vec3 pos = glm::vec3(cur_rob->o.x, 0.0f, cur_rob->o.y);
			delete cur_rob;
			cur_rob = nullptr;
			cur_mode = 1 - cur_mode;
			placeObst(pos);
		}
		break;
	} 
}

void scaleCurrentObstacle(GLfloat xs, GLfloat ys, GLfloat dt) {
	switch (cur_mode) {
	case 0:
		//1 - dt is a lerp
		if (cur_ob != nullptr) {
            Circ * cur_cob = dynamic_cast<Circ *>(cur_ob);
			cur_cob->r *= (xs == 1.f ? ys : xs)*dt + (1 - dt);
		}
		break;
	case 1:
		if (cur_ob != nullptr) {
            Rect * cur_rob = dynamic_cast<Rect *>(cur_ob);
			cur_rob->w *= xs*dt + (1 - dt);
			cur_rob->h *= ys*dt + (1 - dt);
		}
		break;
	}
}

void movePlayer(GLfloat dx, GLfloat dy, GLfloat dt) {
    player->o.x += dx*dt;
    player->o.y += dy*dt;
    init_planning_vis();//really I just need to reinit the circ obstacles :P
}

void moveCurrentObstacle(GLfloat dx, GLfloat dy, GLfloat dt) {
	switch (cur_mode) {
	case 0:
		if (cur_ob != nullptr) {
            Circ * cur_cob = dynamic_cast<Circ *>(cur_ob);
			cur_cob->o.x += dx*dt;
			cur_cob->o.y += dy*dt;
		}
		break;
	case 1:
		if (cur_ob != nullptr) {
            Rect * cur_rob = dynamic_cast<Rect *>(cur_ob);
			cur_rob->o.x += dx*dt;
			cur_rob->o.y += dy*dt;
		}
		break;
	}
}
