#include <string>
#include <iostream>

#include "engine.h"
#include "shader.h"
#include "resources.h"

Engine::Engine():_window(520, 360, "Turtle-OS"){

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
    load_texture("data/cat.jpg");

    float scale = 5.0f;
    // Main render loop
    while (!_window.shouldClose()) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

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
