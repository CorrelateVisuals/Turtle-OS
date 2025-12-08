#include <string>
#include <iostream>

#include "engine.h"
#include "shader.h"

TurtleOS::TurtleOS():_window(520, 360, "Turtle-OS"){

}

void TurtleOS::create_shaders(){
    // Load shader sources from files
    const std::string vert_src = load_shader_file("src/shaders/vertex.glsl");
    const std::string frag_src = load_shader_file("src/shaders/fragment.glsl");

    // Compile and link shader program
    const GLuint shaderProgram = shader_create_program(vert_src, frag_src);
    if (!shaderProgram) {
        throw std::runtime_error("Failed to create shader program");
    }

    glUseProgram(shaderProgram);
}

void TurtleOS::run(){
    create_shaders();
    // Main render loop
    while (!_window.shouldClose()) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Pass vertex positions to the shader
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, TRIANGLE_VERTICES.data());
        glEnableVertexAttribArray(0);

        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(TRIANGLE_VERTICES.size() / 2));

        // Swap buffers and poll events
        _window.swapBuffers();
        _window.pollEvents();
    }
};
