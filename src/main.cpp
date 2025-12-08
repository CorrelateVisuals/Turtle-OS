#include "window.h"
#include "shader.h"
#include <GLES2/gl2.h>
#include <array>
#include <iostream>

// Triangle vertices
constexpr std::array<float, 6> TRIANGLE_VERTICES = {
     0.0f,  0.8f,
    -0.8f, -0.8f,
     0.8f, -0.8f
};

int main() {
    Window window(640, 480, "Triangle Example");

    if (!window.getHandle()) {
        std::cerr << "Failed to create window\n";
        return -1;
    }

    const char* vert_path = "src/shaders/vertex.glsl";
    const char* frag_path = "src/shaders/fragment.glsl";

    char* vert_src = load_shader_file(vert_path);
    char* frag_src = load_shader_file(frag_path);

    GLuint shaderProgram = shader_create_program(vert_src, frag_src);

    free(vert_src);
    free(frag_src);

    glUseProgram(shaderProgram);


    // Main render loop
    while (!window.shouldClose()) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Setup triangle vertices
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, TRIANGLE_VERTICES.data());
        glEnableVertexAttribArray(0);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}
