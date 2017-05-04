#include "main.hpp"

/* some of this might look like stuff from learnopengl.com ... probably not much anymore 
   I have been changing it a lot. 
   Oh, and I think there's a bit from github.com/Polytonic/glitter or maybe from the
   stb github repo... It's mostly just stuff to interface with libraries (e.g. GLFW, 
   stb, glad, etc. ) */
//need to pull stuff out of main into either a Scene or Renderer
//refactor. refactor, refactor
int main() {
    GLFWwindow * window = init_window_context();
    if (window == nullptr)
        return kill_app(EXIT_FAILURE);

	/* Management */
    Gtime::init_stack(1);
    game_loop_clock = &(Gtime::stack[0]);

	/* Shaders */
	Shader * cube_shader = new Shader(
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/cube.vert").c_str(), 
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/cube.frag").c_str());
	Shader * flat_shader = new Shader(
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/flat.vert").c_str(),
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/flat.frag").c_str());
	Shader * lamp_shader = new Shader(
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/lamp.vert").c_str(),
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/lamp.frag").c_str());

    //there's a reason I said it should be in the scene. :p
	cam = new Camera();

	/* Objects */
	// cubes
	const GLuint num_buf = 2;
	GLuint VAO[num_buf]; // should probably make this dynamically resizable... / managed
	glGenVertexArrays(num_buf, VAO);
	GLuint VBO[num_buf];
	glGenBuffers(num_buf, VBO);

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
	GLuint light_VAO;
	glGenVertexArrays(1, &light_VAO);
	glBindVertexArray(light_VAO);
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
		case 4: tex_format = GL_RGBA;      break;
        default:
        case 3: tex_format = GL_RGB;       break;
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
		case 4: tex_format = GL_RGBA;      break;
        default:
        case 3: tex_format = GL_RGB;       break;
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
	ai::init(std::vector<BoundingVolume *>(), std::vector<Object *>(), std::vector<Object *>(), std::vector<Object *>());
	GMP::replan(ai::std_NNai, game_loop_clock);

	//change properties for the path -- instantiate cube visualizations
	//init_planning_vis();

	/* lighting */
	glm::vec3 light_diffuse = glm::vec3(0.5f, 0.5f, 0.5f); // Decrease the influence
	glm::vec3 light_ambient = glm::vec3(0.05f, 0.05f, 0.05f); // Low influence
	glm::vec3 light_specular = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 point_light_positions[] = {
		glm::vec3(-8.f,  1.5f,  -8.f),
		glm::vec3(8.f,  1.5f, 8.f),
		glm::vec3(0.f,  1.f, 4.f),
		glm::vec3(5.f,  2.f, -5.f)
	};
	glm::vec3 dir_light_dir = glm::vec3(-0.5f, 1.0f, -0.7f);
	is_flashlight_on = true;

    /* Game Loop */
    game_loop_clock->frame();
	D(std::cout << std::endl << "Entering Game Loop..." << std::endl << std::endl);	
	while (!glfwWindowShouldClose(window)) {
        game_loop_clock->frame();

		ai::animate_agents(ai::std_NNai, game_loop_clock->delta());

		// Callbacks 
		glfwPollEvents();
		handle_input(game_loop_clock->delta());

		// Render 
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// shaders; pass uniforms; draw
		glm::mat4 proj = glm::perspective(glm::radians(cam->fov), (GLfloat)G::WIN_WIDTH / (GLfloat)G::WIN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = cam->getView();
		glm::mat4 model;

		cube_shader->use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_container);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex_container_specular);

		glUniform3f(cube_shader->Uni("material.ambient"), 1.0f, 0.5f, 0.31f);
		glUniform1f(cube_shader->Uni("material.shine"), 32.0f);
		glUniform1i(cube_shader->Uni("material.diffuse"), 0);
		glUniform1i(cube_shader->Uni("material.specular"), 1);

		glUniform3f(cube_shader->Uni("dirLight.direction"), dir_light_dir.x, -dir_light_dir.y, dir_light_dir.z);
		glUniform3f(cube_shader->Uni("dirLight.ambient"), light_ambient.x*0.1f, light_ambient.y*0.1f, light_ambient.z*0.1f);
		glUniform3f(cube_shader->Uni("dirLight.diffuse"), light_diffuse.x*0.1f, light_diffuse.y*0.1f, light_diffuse.z*0.1f);
		glUniform3f(cube_shader->Uni("dirLight.specular"), light_specular.x*0.1f, light_specular.y*0.1f, light_specular.z*0.1f);

		for (GLuint i = 0; i < 4; i++) {
			std::string si = "pointLights[" + std::to_string(i) + "].";
			glUniform3f(cube_shader->Uni(si + "position"), point_light_positions[i].x, point_light_positions[i].y, point_light_positions[i].z);
			glUniform1f(cube_shader->Uni(si + "constant"), 1.0f);
			glUniform1f(cube_shader->Uni(si + "linear"), 0.045f);
			glUniform1f(cube_shader->Uni(si + "quadratic"), 0.0075f);
			glUniform3f(cube_shader->Uni(si + "ambient"), light_ambient.x, light_ambient.y, light_ambient.z);
			glUniform3f(cube_shader->Uni(si + "diffuse"), light_diffuse.x, light_diffuse.y, light_diffuse.z);
			glUniform3f(cube_shader->Uni(si + "specular"), light_specular.x, light_specular.y, light_specular.z);
		}

		glUniform3f(cube_shader->Uni("spotLight.position"), cam->pos.x, cam->pos.y, cam->pos.z);
		glUniform3f(cube_shader->Uni("spotLight.direction"), cam->dir.x, cam->dir.y, cam->dir.z);
		glUniform3f(cube_shader->Uni("spotLight.ambient"), light_ambient.x*is_flashlight_on, light_ambient.y*is_flashlight_on, light_ambient.z*is_flashlight_on);
		glUniform3f(cube_shader->Uni("spotLight.diffuse"), light_diffuse.x*is_flashlight_on, light_diffuse.y*is_flashlight_on, light_diffuse.z*is_flashlight_on);
		glUniform3f(cube_shader->Uni("spotLight.specular"), light_specular.x*is_flashlight_on, light_specular.y*is_flashlight_on, light_specular.z*is_flashlight_on);
		glUniform1f(cube_shader->Uni("spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(cube_shader->Uni("spotLight.fadeOff"), glm::cos(glm::radians(17.5f)));
		glUniform1f(cube_shader->Uni("spotLight.constant"), 1.0f);
		glUniform1f(cube_shader->Uni("spotLight.linear"), 0.007f);
		glUniform1f(cube_shader->Uni("spotLight.quadratic"), 0.0002f);

		glUniformMatrix4fv(cube_shader->Uni("proj"), 1, GL_FALSE, glm::value_ptr(proj));
		glUniformMatrix4fv(cube_shader->Uni("view"), 1, GL_FALSE, glm::value_ptr(view));

		glBindVertexArray(VAO[0]);

		for (GLuint i = 0; i <obj::NR_RECT_IN_CIRC; i++) {
			model = glm::mat4();
			model = glm::translate(model, obj::rect_in_circ_positions[i]);
			model = glm::rotate(model, 
                obj::rect_in_circ_rotation[i].w, 
                glm::vec3(obj::rect_in_circ_rotation[i].x, 
                    obj::rect_in_circ_rotation[i].y, 
                    obj::rect_in_circ_rotation[i].z));
			model = glm::scale(model, glm::vec3(obj::rect_in_circ_scale[i]));
			glUniformMatrix4fv(cube_shader->Uni("model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(cube_shader->Uni("normalMat"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(view * model))));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		for (GLuint i = 0; i < obj::NR_RECT; i++) {
			model = glm::mat4();
			model = glm::translate(model, obj::rect_positions[i]);
			model = glm::scale(model, glm::vec3(obj::rect_scale[i][0], 1.0f , obj::rect_scale[i][1]));
			glUniformMatrix4fv(cube_shader->Uni("model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(cube_shader->Uni("normalMat"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(view * model))));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		for (GLuint i = 0; i < obj::NR_AGENT_TO_DRAW; i++) {
			model = glm::mat4();
			model = glm::translate(model, obj::agent_positions[i]);
			model = glm::rotate(model, 
                obj::agent_rotation[i].w, 
                glm::vec3(obj::agent_rotation[i].x, 
                    obj::agent_rotation[i].y,
                    obj::agent_rotation[i].z));
			model = glm::scale(model, glm::vec3(obj::agent_scale[i]));
			glUniformMatrix4fv(cube_shader->Uni("model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(cube_shader->Uni("normalMat"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(view * model))));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		/* end cube shaders */

		flat_shader->use();
		glUniform3f(flat_shader->Uni("material.ambient"), 1.0f, 0.5f, 0.31f);
		glUniform1f(flat_shader->Uni("material.shine"), 32.0f);

		glUniform3f(flat_shader->Uni("dirLight.direction"), dir_light_dir.x, dir_light_dir.y, dir_light_dir.z);
		glUniform3f(flat_shader->Uni("dirLight.ambient"), light_ambient.x*0.1f, light_ambient.y*0.1f, light_ambient.z*0.1f);
		glUniform3f(flat_shader->Uni("dirLight.diffuse"), light_diffuse.x*0.1f, light_diffuse.y*0.1f, light_diffuse.z*0.1f);
		glUniform3f(flat_shader->Uni("dirLight.specular"), light_specular.x*0.1f, light_specular.y*0.1f, light_specular.z*0.1f);

		for (GLuint i = 0; i < 4; i++) {
			std::string si = "pointLights[" + std::to_string(i) + "].";
			glUniform3f(flat_shader->Uni( si + "position"), point_light_positions[i].x, point_light_positions[i].y, point_light_positions[i].z);
			glUniform1f(flat_shader->Uni( si + "constant"), 1.0f);
			glUniform1f(flat_shader->Uni( si + "linear"), 0.045f);
			glUniform1f(flat_shader->Uni( si + "quadratic"), 0.0075f);
			glUniform3f(flat_shader->Uni( si + "ambient"), light_ambient.x, light_ambient.y, light_ambient.z);
			glUniform3f(flat_shader->Uni( si + "diffuse"), light_diffuse.x, light_diffuse.y, light_diffuse.z);
			glUniform3f(flat_shader->Uni( si + "specular"), light_specular.x, light_specular.y, light_specular.z);
		}

		glUniform3f(flat_shader->Uni( "spotLight.position"), cam->pos.x, cam->pos.y, cam->pos.z);
		glUniform3f(flat_shader->Uni( "spotLight.direction"), cam->dir.x, cam->dir.y, cam->dir.z);
		glUniform3f(flat_shader->Uni( "spotLight.ambient"), light_ambient.x*is_flashlight_on, light_ambient.y*is_flashlight_on, light_ambient.z*is_flashlight_on);
		glUniform3f(flat_shader->Uni( "spotLight.diffuse"), light_diffuse.x*is_flashlight_on, light_diffuse.y*is_flashlight_on, light_diffuse.z*is_flashlight_on);
		glUniform3f(flat_shader->Uni( "spotLight.specular"), light_specular.x*is_flashlight_on, light_specular.y*is_flashlight_on, light_specular.z*is_flashlight_on);
		glUniform1f(flat_shader->Uni( "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(flat_shader->Uni( "spotLight.fadeOff"), glm::cos(glm::radians(17.5f)));
		glUniform1f(flat_shader->Uni( "spotLight.constant"), 1.0f);
		glUniform1f(flat_shader->Uni( "spotLight.linear"), 0.045f);
		glUniform1f(flat_shader->Uni( "spotLight.quadratic"), 0.0075f);

		glUniformMatrix4fv(flat_shader->Uni( "proj"), 1, GL_FALSE, glm::value_ptr(proj));
		glUniformMatrix4fv(flat_shader->Uni( "view"), 1, GL_FALSE, glm::value_ptr(view));

		for (GLuint i = 0; i < obj::NR_CUBES; i++) {
			glUniform3f(flat_shader->Uni("material.diffuse"), obj::cube_diffuse_color[i].x, obj::cube_diffuse_color[i].y, obj::cube_diffuse_color[i].z);
			glUniform3f(flat_shader->Uni("material.specular"), obj::cube_specular_color[i].x, obj::cube_specular_color[i].y, obj::cube_specular_color[i].z);
			model = glm::mat4();
			model = glm::translate(model, obj::cube_positions[i]);
			model = glm::scale(model, glm::vec3(obj::cube_scale[i]));
			glUniformMatrix4fv(flat_shader->Uni("model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(flat_shader->Uni( "normalMat"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(view * model))));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		/* end flat shaders */

		lamp_shader->use();
		glUniformMatrix4fv(lamp_shader->Uni( "proj"), 1, GL_FALSE, glm::value_ptr(proj));
		glUniformMatrix4fv(lamp_shader->Uni("view"), 1, GL_FALSE, glm::value_ptr(view));

		glUniform3f(lamp_shader->Uni("lightColor"), light_specular.x, light_specular.y, light_specular.z);

		for (GLuint i = 0; i < 4; i++) {
			model = glm::mat4();
			model = glm::translate(model, point_light_positions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			glUniformMatrix4fv(lamp_shader->Uni("model"), 1, GL_FALSE, glm::value_ptr(model));

			glBindVertexArray(light_VAO);
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
    return kill_app(EXIT_SUCCESS);
}

GLFWwindow * init_window_context() {
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
    std::string title = "Slime Apocalypse";
    GLFWwindow* window = glfwCreateWindow(G::WIN_WIDTH, G::WIN_HEIGHT, title.c_str(), monitor, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create OpenGL Context" << std::endl;
        return nullptr;
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
        return nullptr;
    }
    D(std::cout << "OK ::: OpenGL " << glGetString(GL_VERSION) << std::endl);

    /* Handle Viewport */
    D(std::cout << "Creating viewport...");
    GLint width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    D(OK());

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return window;
}

//move to Scene 
void handle_input(GLfloat dt) {
    if (UI::keys[GLFW_KEY_W])
        cam->translate_camera(G::CAMERA::FORWARD, dt);
    if (UI::keys[GLFW_KEY_S])
        cam->translate_camera(G::CAMERA::BACKWARD, dt);
    if (UI::keys[GLFW_KEY_A])
        cam->translate_camera(G::CAMERA::LEFT, dt);
    if (UI::keys[GLFW_KEY_D])
        cam->translate_camera(G::CAMERA::RIGHT, dt);

    if (UI::keys[GLFW_KEY_P]) {
        UI::keys[GLFW_KEY_P] = false;
        GMP::replan(ai::std_NNai, game_loop_clock);
    }

    if (UI::keys[GLFW_KEY_F]) {
        UI::keys[GLFW_KEY_F] = false;
        toggle_flashlight();
    }
}

//move to Scene
void toggle_flashlight() {
	is_flashlight_on = !is_flashlight_on;
}
