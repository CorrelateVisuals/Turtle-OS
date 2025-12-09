#pragma once

#include <GLES2/gl2.h>
#include <GLFW/glfw3.h>
#include <string_view>

class Window {
public:
    Window(int width, int height, std::string_view title);
    ~Window();

    bool shouldClose() const;
    void swapBuffers();
    void pollEvents();
    void setKeyCallback(GLFWkeyfun callback);

    GLFWwindow* getHandle() const;

private:
    GLFWwindow* handle;
};
