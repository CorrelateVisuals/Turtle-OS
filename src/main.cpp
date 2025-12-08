#include "window.h"
#include "shader.h"
#include <GLES2/gl2.h>
#include <array>
#include <iostream>
#include <stdexcept>
#include "engine.h"

TurtleOS os;

int main() {
    try {
        // Create window (RAII)
        Window window(640, 480, "Turtle-OS");

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
        os.main_loop(window);

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
        return -1;
    }

    return 0;
}
