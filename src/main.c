#include "window.h"
#include "shader.h"
#include <GLES2/gl2.h>
#include <stdio.h>
#include <stdlib.h>

static const float vertices[] = {
     0.0f,  0.8f,
    -0.8f, -0.8f,
     0.8f, -0.8f
};

int main() {
    Window* win = window_create(640, 480, "Triangle");
    if (!win) return -1;

    // Load shaders from files or strings
    char* vert_src = load_shader_file("src/shaders/vertex.glsl");
    char* frag_src = load_shader_file("src/shaders/fragment.glsl");




    shader_create_program(vert_src, frag_src);

    free(vert_src);
    free(frag_src);

    while (!window_should_close(win)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        window_swap_buffers(win);
        glfwPollEvents();
    }

    window_destroy(win);
    return 0;
}
