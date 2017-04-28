#include "Timer.hpp"

/* GLOBALS */
/* GL */
#include <glad/glad.h>
#include <GLFW/glfw3.h>

std::vector<Timer> stack;
void init_stack(int layers) {
    stack = std::vector<Timer>(layers);
}
void add_now() {
    stack.push_back(Timer());
}
void del_top() {
    GLfloat elapsed = stack.back().frame();
    stack.pop_back();
    return elapsed;
}

GLfloat Timer::Timer() {
    this->frame_abs = (GLfloat)glfwGetTime();
    this->last_delta = 0.f;
    this->paused_time = 0.f;
}
GLfloat Timer::frame() {
    GLfloat prev = this->frame_abs;
    this->frame_abs = (GLfloat)glfwGetTime();
    this->last_delta = this->frame_abs - prev;
    return this->last_delta;
}
GLfloat Timer::abs() {
    return this->frame_abs;
}
GLfloat Timer::delta() {
    return this->last_delta;
}
GLfloat Timer::pause() {
    this->paused_time = (GLfloat)glfwGetTime();
    return  this->paused_time - this->frame_abs;
}
GLfloat Timer::play() {
    GLfloat elapsed = (GLfloat)glfwGetTime() - this->paused_time;
    this->frame_abs += elapsed;
    return elapsed;
}
