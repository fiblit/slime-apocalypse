#pragma warning(push, 0)
#pragma warning(disable: 4055)
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#pragma warning(pop)

#include "Camera.hpp"

//should be in scene if we're going to be honest,
//however, UI is the only thing touching this atm
extern Camera * cam;

namespace UI {
    extern GLfloat cursor_sensitivity;
    extern GLboolean keys[1024];
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
int kill_app(int retVal);
