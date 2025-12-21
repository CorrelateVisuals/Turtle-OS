#include <array>

// x, y,   u, v
constexpr std::array<float, 16> RECT_VERTICES = {
    -0.5f, 0.5f,  0.0f, 1.0f, // Top-left
    -0.5f, -0.5f, 0.0f, 0.0f, // Bottom-left
    0.5f,  -0.5f, 1.0f, 0.0f, // Bottom-right
    0.5f,  0.5f,  1.0f, 1.0f  // Top-right
};

constexpr std::array<unsigned int, 6> INDICES = {0, 1, 2, 2, 3, 0};
