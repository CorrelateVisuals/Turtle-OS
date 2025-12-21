#pragma once

#include <GLES2/gl2.h>
#include <vector>


#include "window.h"
#include "turtle_baseclass.h"

// x, y,   u, v
constexpr std::array<float, 16> RECT_VERTICES = {
    -0.5f,  0.5f,   0.0f, 1.0f,   // Top-left
    -0.5f, -0.5f,   0.0f, 0.0f,   // Bottom-left
     0.5f, -0.5f,   1.0f, 0.0f,   // Bottom-right
     0.5f,  0.5f,   1.0f, 1.0f    // Top-right
};

constexpr std::array<unsigned int, 6> INDICES = {
    0, 1, 2,
    2, 3, 0
};

class Engine{
public:
    Engine();
    ~Engine() = default;
    
    void run();

private:
    Window _window;

    TurtleBaseclass::ZoomEffect _zoomEffect{15.0f, 0.1f, 2.0f};

    GLint _scale_location;
    float _texture_start_scale = 15.0f;
    float _zoom_speed = 0.1f;
    float _texture_final_scale = 2.0f + _zoom_speed ;

    GLuint _current_texture{0};
    std::vector<std::string> _textures;
    int _texture_index{0};

    void load_all_textures();
    void select_texture(int index);
    static void key_call_back(GLFWwindow* window, int key, int seconds, int action, int mods);

    void create_shaders();
};