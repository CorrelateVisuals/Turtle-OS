#include "input.h"

Input::Input(GLFWwindow *window) : _window(window) {}

void Input::poll() {
}

bool Input::is_key_pressed(int key) const {
    return glfwGetKey(_window, key) == GLFW_PRESS;
}
