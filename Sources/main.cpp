#include "main.hpp"

/* some of this might look like stuff from learnopengl.com ... probably not much anymore 
   I have been changing it a lot. 
   Oh, and I think there's a bit from github.com/Polytonic/glitter or maybe from the
   stb github repo... It's mostly just stuff to interface with libraries (e.g. GLFW, 
   stb, glad, etc. ) */
//need to pull stuff out of main into either a Scene or Renderer
//refactor. refactor, refactor
int main() {
    Scene * s = new Scene();
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

//move to Scene
void move_player(GLfloat dx, GLfloat dy, GLfloat dt) {
    player->o.x += dx*dt;
    player->o.y += dy*dt;
    init_planning_vis();//really I just need to reinit the circ obstacles :P
}
