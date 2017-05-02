#include "UI.hpp"
#include "debug.hpp"
#include <iostream>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    UNUSED(window, scancode, mode);
    //Press ESC to close Application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (action == GLFW_PRESS) {
        D(std::cout << "Key Pressed: " << /*glfwGetKeyName(key, key)*/ key << std::endl);
        UI::keys[key] = true;
    }
    else if (action == GLFW_RELEASE) {
        UI::keys[key] = false;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    UNUSED(window);
    static GLboolean focus = false;
    static GLfloat last_x = 0.f;
    static GLfloat last_y = 0.f;

    if (!focus) {
        last_x = (GLfloat)xpos;
        last_y = (GLfloat)ypos;
        focus = true;
    }
    GLfloat x_offset = (GLfloat)xpos - last_x;
    GLfloat y_offset = (GLfloat)ypos - last_y;
    x_offset *= UI::cursor_sensitivity;
    y_offset *= UI::cursor_sensitivity;
    last_x = (GLfloat)xpos;
    last_y = (GLfloat)ypos;
    cam->mouse_rotate_camera(x_offset, y_offset);
}

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset) {
    UNUSED(window, xoffset);
    cam->scroll_zoom_camera((GLfloat)yoffset);
}

int kill_app(int retVal) {
    glfwTerminate();
    std::cout << std::endl << "Application Terminated. With exit value: " << retVal << std::endl;
    D(slow_print(50, 300, "\n\nGoodbye..."));
    D(slow_print(150, 500, "OK"));
    return retVal;
}
