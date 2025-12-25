#include <array>

class Rectangle {
public:
  std::array<float, 16> _vertices = {
      -0.5f, 0.5f,  0.0f, 1.0f, // Top-left
      -0.5f, -0.5f, 0.0f, 0.0f, // Bottom-left
      0.5f,  -0.5f, 1.0f, 0.0f, // Bottom-right
      0.5f,  0.5f,  1.0f, 1.0f  // Top-right
  };

  std::array<unsigned int, 6> _indices = {0, 1, 2, 2, 3, 0};
};
