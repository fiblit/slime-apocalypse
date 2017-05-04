#include "main.hpp"

/* some of this might look like stuff from learnopengl.com ... probably not much anymore 
   I have been changing it a lot. 
   Oh, and I think there's a bit from github.com/Polytonic/glitter or maybe from the
   stb github repo... It's mostly just stuff to interface with libraries (e.g. GLFW, 
   stb, glad, etc. ) */
//need to pull stuff out of main into either a Scene or Renderer
//refactor. refactor, refactor

using namespace mcl;

int main() {
    GLFWwindow * window = init_window_context();
    if (window == nullptr)
        return kill_app(EXIT_FAILURE);

	/* Management */
    G::time::init_stack(1);
    game_loop_clock = &(G::time::stack[0]);

	/* Shaders */
	Shader * cube_shader = new Shader();
	scene->shaders[TEXTURE] = cube_shader;
	Shader * flat_shader = new Shader();
	scene->shaders[FLAT] = flat_shader;
	Shader * lamp_shader = new Shader();
	scene->shaders[LIGHT] = lamp_shader;

	cube_shader->init_from_strings(
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/cube.vert").c_str(), 
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/cube.frag").c_str());
	flat_shader->init_from_strings(
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/flat.vert").c_str(),
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/flat.frag").c_str());
	lamp_shader->init_from_strings(
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/lamp.vert").c_str(),
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/lamp.frag").c_str());

    //there's a reason I said it should be in the scene. :p
	cam = new Camera();

	// initialize the scene
	scene = new Scene();
	scene->camera = cam;

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
	glBindTexture(GL_TEXTURE_2D, 1);*/

	/* Path Planning */
	init_planning();
	replan();

	//change properties for the path -- instantiate cube visualizations
	//init_planning_vis();

    /* Game Loop */
    game_loop_clock->frame();
	D(std::cout << std::endl << "Entering Game Loop..." << std::endl << std::endl);	
	while (!glfwWindowShouldClose(window)) {
        game_loop_clock->frame();

		//animate_agents(game_loop_clock->delta());

		// Callbacks 
		glfwPollEvents();
		handle_input(game_loop_clock->delta());

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

//move to GMP
/*
global references: none
side effects:
    next_node is updated
    speed may be updated
    agent_now may be updated

*/
void lookahead(glm::vec2 * agent_now, glm::vec2 * next_node, Agent * a, float * speed, float dt) {
    if (a->completed_nodes < static_cast<int>(a->plan->size())) {
        *next_node = (*a->plan)[a->completed_nodes]->data;

        while (a->completed_nodes + 1 < static_cast<int>(a->plan->size())
            && a->cspace->line_of_sight(*agent_now, (*a->plan)[a->completed_nodes + 1]->data)) {
            a->completed_nodes++;
            *next_node = (*a->plan)[a->completed_nodes]->data;
        }

        float dist = glm::distance(*next_node, *agent_now);
        if (dist < 0.1f) {
            a->completed_nodes++;
            //not sure why I have this, as this will, like, never happen.
            if (dist < dt*(*speed)) {//detects overshooting
                *agent_now = *next_node;//move to the node, but not past...
                *speed -= dist / dt;
            }
        }
    }
    else
        *next_node = (*a->plan)[a->plan->size() - 1]->data;
}

//move to LMP
void ttc_forces_(double ttc, glm::vec2 * avoid_force, glm::vec2 dir) {
    dir /= glm::length(dir);

    double t_h = 3.0;//seconds
    double mag = 0;
    if (ttc >= 0 && ttc <= t_h)
        mag = (t_h - ttc) / (ttc + 0.001);
    mag = mag > 20 ? 20 : mag;
    *avoid_force += glm::vec2(mag * dir.x, mag * dir.y);
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

//move to LMP
void boid_forces(Agent * a, glm::vec2 * boid_force) {
    const float boid_speed = 1.2f;

    glm::vec2 avg_vel(0, 0), avg_pos(0, 0), avg_dir(0, 0);
    GLfloat cohes_r_look = 1.0f, align_r_look = 1.0f, separ_r_look = .5f;//limit to search for forces for boidlings
    GLfloat ff0_r = 2.0f;//radius of following force of 0
    GLfloat ff1_r = 10.0f;//radius of following force of 1 towards leader
    glm::vec2 align_force, cohesion_force, follow_force, spread_force;

    for (int i = 0; i < static_cast<int>(boidlings.size()); i++) {
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
    align_force = (avg_vel - a->vel) * boid_speed;

    /* cohesion force */
    //average cohesion; pull towards that
    cohesion_force = avg_pos - a->bv->o;

    /* follow force */
    //distance to leader, weak close, strong far
    glm::vec2 toLeader = player->o - a->bv->o;
    float dist2leader = glm::dot(toLeader, toLeader);
    if (dist2leader < ff0_r * ff0_r)
        follow_force = glm::vec2(0);
    else
        follow_force = toLeader * (dist2leader - ff0_r) / (ff1_r - ff0_r);

    /* separation force */
    //force from inverted direction of nearest neighbours
    for (int i = 0; i < static_cast<int>(boidlings.size()); i++) {
        Agent * boid = boidlings[i];
        if (boid == a)
            continue;
        glm::vec2 toBoid = boid->bv->o - a->bv->o;
        float dist2boid = glm::dot(toBoid, toBoid);

        if (dist2boid < separ_r_look * separ_r_look)
            spread_force += -toBoid / (10*sqrt(dist2boid));
    }

    (*boid_force) += align_force + cohesion_force + follow_force + spread_force;
    if (glm::dot(*boid_force, *boid_force) > 20 * 20) {
        *boid_force /= glm::length(*boid_force);
        *boid_force *= 20.f;
    }
}

//move to LMP(?)
/*
global references:
    lookahead()
    G::WITH_TTC_GRID
    agents
    LMP::ttc()
    ttc_forces()
    boid_forces()
side effects:
    
*/
//needs to be refactored
void force_agents(GLfloat dt, Agent * a, int i, int j, std::vector<Agent *> grid_new[100][100]) {
    
    float speed = 1.0f; // x m/s
    glm::vec2 agent_now = a->bv->o;
    glm::vec2 next_node;
    glm::vec2 goal_vel;
    if (a->plan != nullptr && 0 < a->plan->size()) {
        lookahead(&agent_now, &next_node, a, &speed, dt);
        goal_vel = (next_node - agent_now) / glm::distance(next_node, agent_now) * (speed /* * dt */);
    }
    else {
        next_node = agent_now;
        goal_vel = glm::vec2(0);
    }

    /* ttc - approximate */
    glm::vec2 goal_force;
    if (a->boid)
        goal_force = glm::vec2(0);
    else
        goal_force = 2.0f*(goal_vel - a->vel);

    glm::vec2 sum_force = goal_force;

    glm::vec2 avoid_force(0), boid_force(0);

    if (G::WITH_TTC_GRID) {
        for (int k = (0 < i - 4) ? i - 4 : 0; k < ((100 > i + 4) ? i + 4 : 100); k++) { // spatial search
            for (int l = (0 < j - 4) ? j - 4 : 0; l < ((100 > j + 4) ? j + 4 : 100); l++) {
                for (Agent * b : agents[k][l]) {
                    if (a == b)
                        continue;

                    double ttc = LMP::ttc(a->bv, a->vel, b->bv, b->vel);

                    if (ttc > 10)
                        continue;

                    //suggestion to handle collisions
                    //float originalAr = static_cast<Circ *>(a->bv)->r;
                    //float originalBr = static_cast<Circ *>(b->bv)->r;
                    ttc_forces(a, b, ttc, &avoid_force);
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

            ttc_forces(a, b, ttc, &avoid_force);
        }
    }

    for (Circ * c : obst_bounds) {
        double ttc = LMP::ttc(a->bv, a->vel, c, glm::vec2(0));

        if (ttc > 10)
            continue;

        ttc_forces(a, c, ttc, &avoid_force);
    }

    if (a->boid) {
        boid_forces(a, &boid_force);
    }

    sum_force += avoid_force + boid_force;
    a->vel += sum_force * dt;
    a->bv->o += a->vel * dt;

    if (G::WITH_TTC_GRID) {
        int x = static_cast<int>(a->bv->o.x * 5 + 50);
        if (x < 0) x = 0;
        if (x > 99) x = 99;
        int y = static_cast<int>(a->bv->o.y * 5 + 50);
        if (y < 0) y = 0;
        if (y > 99) y = 99;
        (grid_new)[x][y].push_back(a);
    }
}

//move to AI/planner
void animate_agents(GLfloat dt) {
    std::vector<Agent *> grid_new[100][100];
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            grid_new[i][j] = std::vector<Agent *>();
        }
    }

    if (G::WITH_TTC_GRID)
        for (int i = 0; i < 100; i++)
            for (int j = 0; j < 100; j++)
                for (Agent * a : agents[i][j])
                    force_agents(dt, a, i, j, grid_new);
    else
        for (Agent * a : agents_old)
            force_agents(dt, a, -1, -1, nullptr);

    if (G::WITH_TTC_GRID)
        for (int i = 0; i < 100; i++)
            for (int j = 0; j < 100; j++)
                agents[i][j] = grid_new[i][j];

    int agent_mesh_drawn = 0;
    if (G::WITH_TTC_GRID) { //EVERY SINGLE ONE OF THESE COULD HAVE THEIR INTERNALS AS FUNCTIONS
        for (int c = 0; c < 100; c++) {
            for (int r = 0; r < 100; r++) {
                for (Agent * a : agents[c][r]) {
                    GLuint step;
                    if (a->vt == agent::volume_type::RECT)
                        step = 1;
                    else
                        step = cylinder_res;

                    for (GLuint i = 0; i < step; i++)
                        obj::agent_positions[i + agent_mesh_drawn] = glm::vec3(a->bv->o.x, 0.0f + 0.001f*(i /*+ offset*/), a->bv->o.y);
                    agent_mesh_drawn += step;
                }
            }
        }
    }
    else {
        for (Agent * a : agents_old) {
            GLuint step;
            if (a->vt == agent::volume_type::RECT)
                step = 1;
            else
                step = cylinder_res;

            for (GLuint i = 0; i < step; i++)
                obj::agent_positions[i + agent_mesh_drawn] = glm::vec3(a->bv->o.x, 0.0f + 0.001f*(i /*+ offset*/), a->bv->o.y);
            agent_mesh_drawn += step;
        }
    }
}

//move AI/planner
//refactor
void init_planning() {
	cur_ob = nullptr;
    switch (G::SCENARIO) {
    case G::SCENE::WALL:
        player = new Circ(glm::vec2(5.f, 0.f), .5f);
        break;
    case G::SCENE::DEFAULT:
    case G::SCENE::DEADEND:
    case G::SCENE::NO_BOID:
    case G::SCENE::MAZE:
    default:
        player = new Circ(glm::vec2(0.f), .5f);
        break;
    }

    double boidling_size;
    glm::vec2 boid_spawn, boidling_ranks, boidling_offset, boidling_spacing;
    double agent_size;
    glm::vec2 spacing, group_size, offset, start1, start2, goal1, goal2;

    switch (G::SCENARIO) {
    case G::SCENE::WALL:
        agent_size = .05;
        spacing = glm::vec2(.25, .25);
        group_size = glm::vec2(21, 21);
        offset = glm::vec2(21. / 2. - .5, 21. / 2. - .5);
        start1 = glm::vec2(-7., -7.);
        start2 = glm::vec2(7., 7.);
        goal1 = glm::vec2(7., 7.);
        goal2 = glm::vec2(-7., -7.);

        boidling_size = .05;
        boid_spawn = glm::vec2(6.0f, 0.0f);
        boidling_ranks = glm::vec2(9, 9);
        boidling_offset = glm::vec2(5. / 2. - .5, 5. / 2. - .5);
        boidling_spacing = glm::vec2(.3f, .3f);
        break;
    case G::SCENE::NO_BOID:
        agent_size = .05;
        spacing = glm::vec2(.25, .25);
        group_size = glm::vec2(21, 21);
        offset = glm::vec2(21. / 2. - .5, 21. / 2. - .5);
        start1 = glm::vec2(-7., -7.);
        start2 = glm::vec2(-7., 7.);
        goal1 = glm::vec2(7., 7.);
        goal2 = glm::vec2(7., -7.);

        boidling_size = 0;
        boidling_ranks = glm::vec2(0, 0);
        break;
    case G::SCENE::DEFAULT:
    case G::SCENE::DEADEND:
    case G::SCENE::MAZE:
    default:
        /* PLAY WITH THESE */
        agent_size = .05;
        spacing = glm::vec2(.25, .25);
        group_size = glm::vec2(21, 21);
        offset = glm::vec2(21. / 2. - .5, 21. / 2. - .5);
        start1 = glm::vec2(-7., -7.);
        start2 = glm::vec2(7., 7.);
        goal1 = glm::vec2(7., 7.);
        goal2 = glm::vec2(-7., -7.);

        boidling_size = .05;
        boid_spawn = glm::vec2(-2.0f, 0.0f);
        boidling_ranks = glm::vec2(9, 9);
        boidling_offset = glm::vec2(5. / 2. - .5, 5. / 2. - .5);
        boidling_spacing = glm::vec2(.3f, .3f);
        /* */
        break;
    }

    if (G::WITH_TTC_GRID)
        for (int i = 0; i < 100; i++)
            for (int j = 0; j < 100; j++)
                agents[i][j] = std::vector<Agent *>();
    else
        agents_old = std::vector<Agent *>(static_cast<int>(group_size.x * group_size.y * 2 + boidling_ranks.x * boidling_ranks.y));
    //only use circs since I did not expand ttc for my rects yet

    for (int i = 0; i < group_size.x; i++) {
        for (int j = 0; j < group_size.y; j++) {
            glm::vec2 o(start1.x + spacing.x * (i - offset.x), start1.y + spacing.y * (j - offset.y));
            Agent * a = new Agent(agent::volume_type::CIRC,
                new Circ(o, static_cast<float>(agent_size)),
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
                agents_old[static_cast<int>(i + group_size.x * j)] = a;
            }
        }
    }

    for (int i = 0; i < group_size.x; i++) {
        for (int j = 0; j < group_size.y; j++) {
            glm::vec2 o(start2.x + spacing.x * (i - offset.x), start2.y + spacing.y * (j - offset.y));
            Agent * a = new Agent(agent::volume_type::CIRC,
                new Circ(o, static_cast<float>(agent_size)),
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
                agents_old[static_cast<int>(i + group_size.x * j + group_size.x * group_size.y)] = a;
            }
        }
    }

    for (int i = 0; i < boidling_ranks.x; i++) {
        for (int j = 0; j < boidling_ranks.y; j++) {
            glm::vec2 o(boid_spawn.x + boidling_spacing.x * (i - boidling_offset.x), boid_spawn.y + boidling_spacing.y * (j - boidling_offset.y));
            Agent * a = new Agent(agent::volume_type::CIRC, new Circ(o, static_cast<float>(boidling_size)), glm::vec2(0));
            a->boid = true;

            if (G::WITH_TTC_GRID) {
                int x = static_cast<int>(o.x * 5 + 50);
                int y = static_cast<int>(o.y * 5 + 50);

                agents[x][y].push_back(a);
                NUM_AGENTS++;
            }
            else {
                agents_old[static_cast<int>(i + group_size.x * j + 2 * group_size.x * group_size.y)] = a;
            }

            boidlings.push_back(a);
        }
    }
    
    switch (G::SCENARIO) {
    case G::SCENE::DEFAULT:
    case G::SCENE::NO_BOID:
        obst_bounds = std::vector<Circ *>(2);
        obst_bounds[0] = new Circ(glm::vec2(-3.0f, -3.0f), 2.0f);
        obst_bounds[1] = new Circ(glm::vec2(-2.0f, 6.0f), 1.0f);
        rect_bounds = std::vector<Rect *>();
        obst_bounds.push_back(player);
        break;
    case G::SCENE::DEADEND:
        obst_bounds = std::vector<Circ *>(9);
        for (int i = 0; i < 5; i++)
            obst_bounds[i] = new Circ(glm::vec2(-3.5f, i - 7.5), .49f);
        for (int i = 0; i < 4; i++)
            obst_bounds[i + 5] = new Circ(glm::vec2(i - 7.5, -3.5f), .49f);

        rect_bounds = std::vector<Rect *>();
        obst_bounds.push_back(player);
        break;
    case G::SCENE::WALL:
        obst_bounds = std::vector<Circ *>(36);
        for (int sign = -1; sign < 2; sign += 2)
            for (float i = 0; i < 18; i++)
                obst_bounds[static_cast<int>(i + ((1+sign)/2)*18)] =
                    new Circ(glm::vec2(-sign * (i / 2.f + 1.f), sign * (i / 2.f + 1.f)), .5f);
        rect_bounds = std::vector<Rect *>();
        obst_bounds.push_back(player);
        break;
    case G::SCENE::MAZE:
        obst_bounds = std::vector<Circ *>(2);
        obst_bounds[0] = new Circ(glm::vec2(-3.0f, -3.0f), 2.0f);
        obst_bounds[1] = new Circ(glm::vec2(-2.0f, 6.0f), 1.0f);
        rect_bounds = std::vector<Rect *>(10);
        {int NR = 0;
        //fix these at some point..
        rect_bounds[NR] = new Rect(glm::vec2(3.0f, 8.0f), 0.1f, 4.0f); NR++;
        rect_bounds[NR] = new Rect(glm::vec2(-5.0f, -1.0f), 5.5f, 0.1f); NR++;
        rect_bounds[NR] = new Rect(glm::vec2(3.0f, -7.0f), 0.1f, 6.0f); NR++;
        rect_bounds[NR] = new Rect(glm::vec2(3.0f, 0.0f), 0.1f, 4.0f); NR++;
        rect_bounds[NR] = new Rect(glm::vec2(-6.5f, -2.0f), 0.1f, 8.0f); NR++;//
        rect_bounds[NR] = new Rect(glm::vec2(6.0f, 2.0f), 6.0f, 0.1f); NR++;
        rect_bounds[NR] = new Rect(glm::vec2(-6.0f, 6.0f), 3.0f, 0.1f); NR++;
        rect_bounds[NR] = new Rect(glm::vec2(-6.0f, 6.0f), 0.1f, 3.0f); NR++;
        rect_bounds[NR] = new Rect(glm::vec2(0.0f, 4.0f), 0.1f, 4.0f); NR++;
        rect_bounds[NR] = new Rect(glm::vec2(-3.0f,    6.0f),    6.0f,   0.1f); NR++;//
        }
        obst_bounds.push_back(player);
        break;
    }
}

//move to AI/planner? Scene? Renderer?
//refactor into other functions or subfunctions
void init_planning_vis() {
    Agent * debug_agent;
    if (!G::WITH_TTC_GRID)
        debug_agent = agents_old[selected_agent_debug];

    //for the sake of the uniform grid working :P
    if (!G::WITH_TTC_GRID && debug_agent->prm != nullptr) {
        std::vector<Node<glm::vec2> *> * verts = debug_agent->prm->roadmap->vertices;
        obj::NR_CUBES = static_cast<GLuint>(verts->size() + 1);
        obj::cube_positions = new glm::vec3[obj::NR_CUBES];
        obj::cube_scale = new float[obj::NR_CUBES];
        obj::cube_diffuse_color = new glm::vec3[obj::NR_CUBES];
        obj::cube_specular_color = new glm::vec3[obj::NR_CUBES];

        for (GLuint i = 0; i < obj::NR_CUBES - 1; i++) {
            Node<glm::vec2> * v = verts->at(i);
            obj::cube_positions[i] = glm::vec3(v->data.x, -2.0f, v->data.y);
            if (i == 0) {//start = blue
                obj::cube_scale[i] = 1.0f;
                obj::cube_diffuse_color[i] = glm::vec3(0.0f, 0.0f, 1.0f);
                obj::cube_specular_color[i] = glm::vec3(1.0f, 1.0f, 1.0f);
                obj::cube_positions[i] = glm::vec3(debug_agent->start.x, -2.0f, debug_agent->start.y);
            }
            else if (i == 1) {//goal = red
                obj::cube_scale[i] = 1.0f;
                obj::cube_diffuse_color[i] = glm::vec3(1.0f, 0.0f, 0.0f);
                obj::cube_specular_color[i] = glm::vec3(1.0f, 1.0f, 1.0f);
                obj::cube_positions[i] = glm::vec3(debug_agent->goal.x, -2.0f, debug_agent->goal.y);
            }
            else if (find(debug_agent->plan->begin(), debug_agent->plan->end(), v) != debug_agent->plan->end()) {
                obj::cube_scale[i] = 0.75f;
                obj::cube_diffuse_color[i] = glm::vec3(0.0f, 1.0f, 0.0f);
                obj::cube_specular_color[i] = glm::vec3(1.0f, 1.0f, 1.0f);
            }
            else {
                obj::cube_scale[i] = 0.5f;
                obj::cube_diffuse_color[i] = glm::vec3(0.4f, 0.4f, 0.4f);
                obj::cube_specular_color[i] = glm::vec3(1.0f, 1.0f, 1.0f);
            }
        }

        //std::vector<std::pair<Node<glm::vec2> *, Node<glm::vec2> *>> * all_edges = prm->roadmap->all_edges;
        //glBufferData(,,)
    }
    else {
        obj::NR_CUBES = 1;
        obj::cube_positions = new glm::vec3[obj::NR_CUBES];
        obj::cube_scale = new float[obj::NR_CUBES];
        obj::cube_diffuse_color = new glm::vec3[obj::NR_CUBES];
        obj::cube_specular_color = new glm::vec3[obj::NR_CUBES];
    }

    // add ground
    obj::cube_scale[obj::NR_CUBES - 1] = 20.0f;
    obj::cube_positions[obj::NR_CUBES - 1] = glm::vec3(0.0f, -12.0f, 0.0f);
    obj::cube_diffuse_color[obj::NR_CUBES - 1] = glm::vec3(0.3f, 0.5f, 0.3f);
    obj::cube_specular_color[obj::NR_CUBES - 1] = glm::vec3(1.0f, 1.0f, 1.0f);


    // obstacles -- could be polymorphised :p
    //add circs
    obj::NR_RECT_IN_CIRC = static_cast<GLuint>(cylinder_res * obst_bounds.size());
    obj::rect_in_circ_positions = new glm::vec3[obj::NR_RECT_IN_CIRC];
    obj::rect_in_circ_scale = new float[obj::NR_RECT_IN_CIRC];
    obj::rect_in_circ_rotation = new glm::vec4[obj::NR_RECT_IN_CIRC];
    for (GLuint i = 0; i < obj::NR_RECT_IN_CIRC; i++) {
        obj::rect_in_circ_positions[i] = glm::vec3(obst_bounds[(int)i / cylinder_res]->o.x, 0.0f - 0.001f*i, obst_bounds[(int)i / cylinder_res]->o.y);
        obj::rect_in_circ_scale[i] = obst_bounds[(int)i / cylinder_res]->r * static_cast<float>(sqrt(2));
        obj::rect_in_circ_rotation[i] = glm::vec4(0.0f, 1.0f, 0.0f, glm::radians(360.0f / cylinder_res * (i%cylinder_res)));
    }

    //add rects
    obj::NR_RECT = static_cast<GLuint>(rect_bounds.size());
    obj::rect_positions = new glm::vec3[obj::NR_RECT];
    obj::rect_scale = new glm::vec2[obj::NR_RECT];
    for (GLuint i = 0; i < obj::NR_RECT; i++) {
        obj::rect_positions[i] = glm::vec3(rect_bounds[i]->o.x, 0.0f, rect_bounds[i]->o.y);
        obj::rect_scale[i] = glm::vec2(rect_bounds[i]->w, rect_bounds[i]->h);
    }

    //add agent -- could be polymorphised -- :P
    count_NR_AGENT_TO_DRAW();

    init_agent_data();
}

//with planning vis
void inc_NR_AGENT_TO_DRAW(std::vector<Agent *> agents) {
    for (Agent * a : agents)
        if (a->vt == agent::volume_type::RECT)
            obj::NR_AGENT_TO_DRAW++;
        else
            obj::NR_AGENT_TO_DRAW += cylinder_res;
}
void count_NR_AGENT_TO_DRAW() {
    obj::NR_AGENT_TO_DRAW = 0;
    if (G::WITH_TTC_GRID)
        for (int i = 0; i < 100; i++)
            for (int j = 0; j < 100; j++)
                inc_NR_AGENT_TO_DRAW(agents[i][j]);
    else
        inc_NR_AGENT_TO_DRAW(agents_old);
}

//with planning vis
void update_agents_bin(std::vector<Agent *> agents, GLuint * offset) {
    for (Agent * a : agents) {
        GLuint step;
        if (a->vt == agent::volume_type::RECT)
            step = 1;
        else
            step = cylinder_res;

        for (GLuint i = 0; i < step; i++) {
            obj::agent_positions[i + *offset] =
                glm::vec3(a->bv->o.x, 0.0f + 0.001f*i, a->bv->o.y);
            obj::agent_rotation[i + *offset] =
                glm::vec4(
                    0.f, 1.f, 0.f,
                    glm::radians(360.f / cylinder_res * (i % cylinder_res)));
            if (a->vt == agent::volume_type::CIRC)
                obj::agent_scale[i + *offset] =
                    static_cast<Circ *>(a->bv)->r * static_cast<GLfloat>(sqrt(2));
            else
                obj::agent_scale[i + *offset] = static_cast<Rect *>(a->bv)->w;
        }
        *offset += step;
    }
}
void init_agent_data() {
    obj::agent_positions = new glm::vec3[obj::NR_AGENT_TO_DRAW];
    obj::agent_scale = new GLfloat[obj::NR_AGENT_TO_DRAW];
    obj::agent_rotation = new glm::vec4[obj::NR_AGENT_TO_DRAW];

    GLuint offset = 0;
    if (G::WITH_TTC_GRID) {//todo internals -> function / macro / preprocess
        for (int c = 0; c < 100; c++)
            for (int r = 0; r < 100; r++)
                //the grid is why I have to pass offset in like this
                update_agents_bin(agents[c][r], &offset);
    }
    else
        update_agents_bin(agents_old, &offset);
}

//move to AI/planner
//refactor
void replan() {
    game_loop_clock->pause();
	switch (cur_mode) {
	case 0:
		if (cur_ob != nullptr) {
            Circ * cur_cob = dynamic_cast<Circ *>(cur_ob);
			obst_bounds.push_back(cur_cob);
			delete cur_cob;
			cur_cob = nullptr;
		}
		break;
	case 1:
		if (cur_ob != nullptr) {
            Rect * cur_rob = dynamic_cast<Rect *>(cur_ob);
			rect_bounds.push_back(cur_rob);
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
                    //    obj::agent_positions[i + positions_updated] = glm::vec3(a->start.x, 0.0f, a->start.y);

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
    bv.reserve(obst_bounds.size() + rect_bounds.size());// + 2 * NUM_AGENTS - 2);
    bv.insert(bv.end(), obst_bounds.begin(), obst_bounds.end());
    bv.insert(bv.end(), rect_bounds.begin(), rect_bounds.end());

    if (G::WITH_TTC_GRID) {
        Cspace_2D * std_cspace = nullptr;
        for (int i = 0; i < 100; i++)
            for (int j = 0; j < 100; j++)
                for (Agent * a : agents[i][j]) {
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
                        a->plan = GMP::find_path_Astar(1.f, a->prm->roadmap);
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
                a->plan = GMP::find_path_Astar(1.f, a->prm->roadmap);
                a->completed_nodes = 0;
            }
        }
    }

    init_planning_vis();
    game_loop_clock->play();
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
        replan();
        init_planning_vis();
    }

    //deprecated for the uniform grid to work
    if (!G::WITH_TTC_GRID && UI::keys[GLFW_KEY_G]) {
        UI::keys[GLFW_KEY_G] = false;
        selected_agent_debug = (selected_agent_debug + 1) % NUM_AGENTS;
        init_planning_vis();
    }
    if (!G::WITH_TTC_GRID && UI::keys[GLFW_KEY_T]) {
        UI::keys[GLFW_KEY_T] = false;
        selected_agent_debug = (selected_agent_debug - 1) % NUM_AGENTS;
        init_planning_vis();
    }

    /* //the dynamic obstacle is buggy with multi-agents for some reason
    if (UI::keys[GLFW_KEY_O]) {
    UI::keys[GLFW_KEY_O] = false;
    place_obst(cam->pos);
    }
    if (UI::keys[GLFW_KEY_M]) {
    UI::keys[GLFW_KEY_M] = false;
    mode_toggle_current_obstacle();
    }
    if (UI::keys[GLFW_KEY_I])
    scale_current_obstacle(1.f, 1.1f, timer->getDelta());
    if (UI::keys[GLFW_KEY_K])
    scale_current_obstacle(1.f, 1/1.1f, timer->getDelta());
    if (UI::keys[GLFW_KEY_J])
    scale_current_obstacle(1 / 1.1f, 1.f, timer->getDelta());
    if (UI::keys[GLFW_KEY_L])
    scale_current_obstacle(1.1f, 1.f, timer->getDelta());
    */
    if (UI::keys[GLFW_KEY_UP])
        move_player(0.f, 1.3f, dt);
    if (UI::keys[GLFW_KEY_DOWN])
        move_player(0.f, -1.3f, dt);
    if (UI::keys[GLFW_KEY_LEFT])
        move_player(-1.3f, 0.f, dt);
    if (UI::keys[GLFW_KEY_RIGHT])
        move_player(1.3f, 0.f, dt);

    if (UI::keys[GLFW_KEY_F]) {
        UI::keys[GLFW_KEY_F] = false;
        toggle_flashlight();
    }
}

//move to Scene
void toggle_flashlight() {
	// TODO: move to Scene
	is_flashlight_on = !is_flashlight_on;
}

//move to Scene
void place_obst(glm::vec3 pos) {
	switch (cur_mode) {
	case 0:
		if (cur_ob != nullptr) {
            Circ * cur_cob = dynamic_cast<Circ *>(cur_ob);
			obst_bounds.push_back(cur_cob);
			init_planning_vis();
			delete cur_cob;
		}
		cur_ob = new Circ(glm::vec2(pos.x, pos.z), 1.0f);
		break;
	case 1:
        if (cur_ob != nullptr) {
            Rect * cur_cob = dynamic_cast<Rect *>(cur_ob);
            rect_bounds.push_back(cur_cob);
            init_planning_vis();
            delete cur_cob;
        }
        cur_ob = new Rect(glm::vec2(pos.x, pos.z), 1.0f, 1.0f);
        break;
	}

    //replan();
}

//deprecated
//move to Scene
void place_goal_node(glm::vec3 pos) {
	agents_old[0]->goal.x = pos.x; agents_old[0]->goal.y = pos.z;
	init_planning_vis();
}

//deprecated
//move to Scene
void place_start_node(glm::vec3 pos) {
    agents_old[0]->start.x = pos.x; agents_old[0]->start.y = pos.z;
	init_planning_vis();
}

//move to Scene
void mode_toggle_current_obstacle() {
	switch (cur_mode) {
	case 0:
		if (cur_ob != nullptr) {
            Circ * cur_cob = dynamic_cast<Circ *>(cur_ob);
			glm::vec3 pos = glm::vec3(cur_cob->o.x, 0.0f, cur_cob->o.y);
			delete cur_cob;
			cur_cob = nullptr;
			cur_mode = 1 - cur_mode;
			place_obst(pos);
		}
		break;
	case 1:
		if (cur_ob != nullptr) {
            Rect * cur_rob = dynamic_cast<Rect *>(cur_ob);
			glm::vec3 pos = glm::vec3(cur_rob->o.x, 0.0f, cur_rob->o.y);
			delete cur_rob;
			cur_rob = nullptr;
			cur_mode = 1 - cur_mode;
			place_obst(pos);
		}
		break;
	} 
}

//move to Scene
void scale_current_obstacle(GLfloat xs, GLfloat ys, GLfloat dt) {
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

//move to Scene
void move_player(GLfloat dx, GLfloat dy, GLfloat dt) {
    player->o.x += dx*dt;
    player->o.y += dy*dt;
    init_planning_vis();//really I just need to reinit the circ obstacles :P
}

//move to Scene
void move_current_obstacle(GLfloat dx, GLfloat dy, GLfloat dt) {
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
