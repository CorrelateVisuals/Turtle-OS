#include <string>
#include <iostream>
#include <filesystem>

#include "engine.h"
#include "shader.h"
#include "resources.h"
#include "image_finder.h"

Engine::Engine():_window("Turtle-OS"){
    ImageFinder finder;
    finder.set_root("data");
    _textures = finder.find_all_images();
    finder.randomize_order(_textures);
    
    select_texture(_texture_index);

    _window.set_key_callback(key_call_back);
}

void Engine::select_texture(int index) {
    if (_textures.empty()) return;

    _texture_index = (index + _textures.size()) % _textures.size();

    std::cout << "Switching to texture: " << _textures[_texture_index] << "\n";
    _texture_start_scale = 15.0f;
    _current_texture = load_texture(_textures[_texture_index]);
    glBindTexture(GL_TEXTURE_2D, _current_texture);
}

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

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
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

    _scale_location = glGetUniformLocation(shaderProgram, "scale");

}

void Engine::run(){
    create_shaders();
    //load_texture("data/cat.jpg");
    glfwSetWindowUserPointer(_window.get_handle(), this);

    // Main render loop
    while (!_window.should_close()) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Update viewport each frame (important on Pinebook!)
        int w, h;
        glfwGetFramebufferSize(_window.get_handle(), &w, &h);
        glViewport(0, 0, w, h);


        glBindTexture(GL_TEXTURE_2D, _current_texture);
        glUniform1f(_scale_location, _texture_start_scale);

        if (_texture_start_scale > _texture_final_scale){
            _texture_start_scale -= _zoom_speed;
        } else {
            _texture_start_scale = _texture_final_scale;
        }

        // Pass vertex positions to the shader
        glEnableVertexAttribArray(0); // pos
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, RECT_VERTICES.data());

        glEnableVertexAttribArray(1); // uv
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, RECT_VERTICES.data() + 2);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, INDICES.data());


        // Swap buffers and poll events
        _window.swap_buffers();
        _window.poll_events();
    }
};



