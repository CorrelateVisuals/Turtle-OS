#ifndef TURTLE_OS_INPUT_H
#define TURTLE_OS_INPUT_H

#include <GLFW/glfw3.h>

class Input {
public:
    explicit Input(GLFWwindow *window);

    void poll();
    bool is_key_pressed(int key) const;

private:
    GLFWwindow *_window;
};

#endif // TURTLE_OS_INPUT_H
