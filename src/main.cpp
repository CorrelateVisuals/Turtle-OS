#include "window.h"
#include "shader.h"
#include <GLES2/gl2.h>
#include <array>
#include <iostream>
#include <stdexcept>

// Triangle vertices (can move to a file later)
constexpr std::array<float, 6> TRIANGLE_VERTICES = {
     0.0f,  0.8f,
    -0.8f, -0.8f,
     0.8f, -0.8f
};

int main() {
    try {
        // Create window (RAII)
        Window window(640, 480, "Bright-OS");

        // Load shader sources from files
        const std::string vert_src = load_shader_file("src/shaders/vertex.glsl");
        const std::string frag_src = load_shader_file("src/shaders/fragment.glsl");

        // Compile and link shader program
        const GLuint shaderProgram = shader_create_program(vert_src, frag_src);
        if (!shaderProgram) {
            throw std::runtime_error("Failed to create shader program");
        }

        glUseProgram(shaderProgram);

        // Main render loop
        while (!window.shouldClose()) {
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // Pass vertex positions to the shader
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, TRIANGLE_VERTICES.data());
            glEnableVertexAttribArray(0);

            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(TRIANGLE_VERTICES.size() / 2));

            // Swap buffers and poll events
            window.swapBuffers();
            window.pollEvents();
        }

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
        return -1;
    }

    return 0;
}
