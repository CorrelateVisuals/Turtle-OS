#ifndef TURTLE_OS_WINDOW_H
#define TURTLE_OS_WINDOW_H

#include <GLFW/glfw3.h>

class Window {
public:
    Window(int width, int height, const char *title);
    ~Window();

    bool should_close() const;
    void request_close();
    void swap_buffers();
    void poll_events();

    GLFWwindow *handle() const;
    void get_framebuffer_size(int &width, int &height) const;

private:
    GLFWwindow *_handle;
};

#endif // TURTLE_OS_WINDOW_H
