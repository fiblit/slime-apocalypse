#include "main.hpp"

using namespace mcl;

int main() {
    GLFWwindow * window = init_window_context();
    if (window == nullptr)
        return kill_app(EXIT_FAILURE);

	/* Management */
    Gtime::init_stack(1);
    Gtime::Timer * game_loop_clock = &(Gtime::stack[0]);

	// initialize the scene
	scene = new Scene();

	/* Shaders */
	Shader * cube_shader = new Shader();
	scene->shaders[TEXTURE] = cube_shader;
	Shader * flat_shader = new Shader();
	scene->shaders[FLAT] = flat_shader;
	Shader * lamp_shader = new Shader();
	scene->shaders[LIGHT] = lamp_shader;

	cube_shader->init_from_files(
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/cube.vert").c_str(), 
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/cube.frag").c_str());
	flat_shader->init_from_files(
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/flat.vert").c_str(),
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/flat.frag").c_str());
	lamp_shader->init_from_files(
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/lamp.vert").c_str(),
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/lamp.frag").c_str());

	/* Objects */
	glGenVertexArrays(1, &scene->bc.scene_vao);

	glGenBuffers(1, &scene->bc.position_vbo);
	glGenBuffers(1, &scene->bc.colors_vbo);
	glGenBuffers(1, &scene->bc.normals_vbo);
	glGenBuffers(1, &scene->bc.faces_ibo);
	glGenBuffers(1, &scene->bc.edges_ibo);

	glBindVertexArray(scene->bc.scene_vao);

	/*glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
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
	glBindVertexArray(0);*/

	/* Load textures */
	/*GLenum tex_format;
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
	glBindTexture(GL_TEXTURE_2D, 1);*/

	/* Path Planning */
    //todo dalton: reintegrate
	//ai::init(std::vector<BoundingVolume *>(), std::vector<Object *>(), std::vector<Object *>(), std::vector<Object *>());
	//GMP::replan(ai::std_NNai, game_loop_clock);

	//change properties for the path -- instantiate cube visualizations
	//init_planning_vis();

	scene->setupTestingObjects();
	scene->enableLightShader();

    /* Game Loop */
    game_loop_clock->frame();
	D(std::cout << std::endl << "Entering Game Loop..." << std::endl << std::endl);	
	while (!glfwWindowShouldClose(window)) {
        game_loop_clock->frame();

        //reintegrate
		//ai::animate_agents(ai::std_NNai, game_loop_clock->delta());

		// Callbacks 
		glfwPollEvents();
		handle_input(game_loop_clock, scene);

		// Render 
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(scene->bc.scene_vao);

		scene->render();

		// Swap the Double Buffer
		glfwSwapBuffers(window);
    }
	
	D(std::cout << std::endl << "Exiting Game Loop..." << std::endl << std::endl);
	

	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, const_cast<GLuint *>(&scene->bc.scene_vao));
	glDeleteBuffers(1, const_cast<GLuint *>(&scene->bc.position_vbo));
	glDeleteBuffers(1, const_cast<GLuint *>(&scene->bc.colors_vbo));
	glDeleteBuffers(1, const_cast<GLuint *>(&scene->bc.normals_vbo));
	glDeleteBuffers(1, const_cast<GLuint *>(&scene->bc.faces_ibo));
	glDeleteBuffers(1, const_cast<GLuint *>(&scene->bc.edges_ibo));

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

void handle_input(Gtime::Timer * clock, Scene * scene) {
    scene->camera->mouse_rotate_camera(UI::cursor_edx, UI::cursor_edy);
    scene->camera->scroll_zoom_camera(UI::d_scroll);

    if (UI::keys[GLFW_KEY_W])
        scene->camera->translate_camera(G::CAMERA::FORWARD, clock->delta());
    if (UI::keys[GLFW_KEY_S])
        scene->camera->translate_camera(G::CAMERA::BACKWARD, clock->delta());
    if (UI::keys[GLFW_KEY_A])
        scene->camera->translate_camera(G::CAMERA::LEFT, clock->delta());
    if (UI::keys[GLFW_KEY_D])
        scene->camera->translate_camera(G::CAMERA::RIGHT, clock->delta());

    if (UI::keys[GLFW_KEY_P]) {
        UI::keys[GLFW_KEY_P] = false;
        GMP::replan(ai::std_NNai, clock);
    }

    if (UI::keys[GLFW_KEY_F]) {
        UI::keys[GLFW_KEY_F] = false;
        scene->toggle_flashlight();
    }
}

int kill_app(int retVal) {
    glfwTerminate();
    std::cout << std::endl << "Application Terminated. With exit value: " << retVal << std::endl;
    D(slow_print(50, 300, "\n\nGoodbye..."));
    D(slow_print(150, 500, "OK"));
    return retVal;
}
