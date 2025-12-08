#include <GLES2/gl2.h>
#include "window.h"
#include "array"

// Triangle vertices (can move to a file later)
constexpr std::array<float, 6> TRIANGLE_VERTICES = {
     0.0f,  0.8f,
    -0.8f, -0.8f,
     0.8f, -0.8f
};

class TurtleOS{
public:
    TurtleOS();
    Window window;

    void run();

private:
    void create_shaders();
};