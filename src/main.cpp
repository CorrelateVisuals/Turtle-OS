#include <iostream>
#include <stdexcept>

#include "engine.h"
#include "geometry.h"

int main(int argc, char *argv[]) {
  
  // TB::Geometry::Vertex tex1;

  // TB::Geometry::Vertex tex3{{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}};

  // TB::Geometry::Shape s{};
  // TB::Geometry::Shape s1{2, 0.5f, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, "new"};
  
 // Image::Texture t1;


  Engine turtle_os(argc, argv);

  try {
    turtle_os.run();

  } catch (const std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
    return -1;
  }
  return 0;
}
