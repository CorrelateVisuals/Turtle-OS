#include <string>
#include <iostream>
#include <filesystem>

#include "engine.h"
#include "shader.h"
#include "resources.h"

Engine::Engine():_window(520, 360, "Turtle-OS"){
    load_all_textures();

    if (_textures.empty()) {
        std::cerr << "No textures found in data/ folder.\n";
    } else {
        select_texture(0);
    }

    _window.setKeyCallback(key_call_back);
}

// ----------------------------------------------------
//   Scan data/ directory and collect texture paths
// ----------------------------------------------------
void Engine::load_all_textures() {
    _textures.clear();

    for (const auto& entry : std::filesystem::directory_iterator("data")) {
        if (entry.path().extension() == ".jpg") {
            _textures.push_back(entry.path().string());
        }
    }

    std::cout << "Loaded " << _textures.size() << " textures.\n";
}

// ----------------------------------------------------
//   Load a texture by index
// ----------------------------------------------------
void Engine::select_texture(int index) {
    if (_textures.empty()) return;

    _texture_index = (index + _textures.size()) % _textures.size();

    std::cout << "Switching to texture: " << _textures[_texture_index] << "\n";

    _current_texture = load_texture(_textures[_texture_index]);
    glBindTexture(GL_TEXTURE_2D, _current_texture);
}

// ----------------------------------------------------
//   Key callback for left/right arrow cycling
// ----------------------------------------------------
void Engine::key_call_back(GLFWwindow* window, int key, int, int action, int) {
    if (action != GLFW_PRESS) return;

    // Retrieve Engine* pointer stored in GLFW window
    Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));

    if (!engine) return;

    if (key == GLFW_KEY_RIGHT) {
        engine->select_texture(engine->_texture_index + 1);
    }
    else if (key == GLFW_KEY_LEFT) {
        engine->select_texture(engine->_texture_index - 1);
    }
}

void Engine::create_shaders(){
    // Load shader sources from files
    const std::string vert_src = load_shader_file("src/shaders/vertex.glsl");
    const std::string frag_src = load_shader_file("src/shaders/fragment.glsl");


    // Compile and link shader program
    const GLuint shaderProgram = shader_create_program(vert_src, frag_src);
    if (!shaderProgram) {
        throw std::runtime_error("Failed to create shader program");
    }

    glUseProgram(shaderProgram);

    _scaleLocation = glGetUniformLocation(shaderProgram, "scale");

}

void Engine::run(){
    create_shaders();
    //load_texture("data/cat.jpg");
    glfwSetWindowUserPointer(_window.getHandle(), this);

    float scale = 5.0f;
    // Main render loop
    while (!_window.shouldClose()) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, _current_texture);
        glUniform1f(_scaleLocation, scale);

        if (scale > 1.0f){
            scale -= 0.01f;
        } else {
            scale = 1.0f;
        }

        // Pass vertex positions to the shader
        glEnableVertexAttribArray(0); // pos
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, RECT_VERTICES.data());

        glEnableVertexAttribArray(1); // uv
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, RECT_VERTICES.data() + 2);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, INDICES.data());


        // Swap buffers and poll events
        _window.swapBuffers();
        _window.pollEvents();
    }
};
