#pragma once

#include <GLES2/gl2.h>
#include <GLFW/glfw3.h>
#include <string_view>

class Window {
public:
    Window(int width, int height, std::string_view title);
    ~Window();

    bool should_close() const;
    void swap_buffers();
    void poll_events();
    void set_key_callback(GLFWkeyfun callback);

    GLFWwindow* get_handle() const;

private:
    GLFWwindow* _handle;
};
