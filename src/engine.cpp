#include "engine.h"

void TurtleOS::main_loop(Window& window){
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
};