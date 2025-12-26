#include <iostream>
#include <stdexcept>

#include "engine.h"
#include "geometry.h"

int main(int argc, char *argv[]) {
  
  Geometry::Vertex tex1;

  Geometry::Vertex tex3{{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}};

  Geometry::Shape s{{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}};
   Geometry::Shape s1{{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, "new", 2, 3.0f};
  


  Engine turtle_os(argc, argv);

  try {
    turtle_os.run();

  } catch (const std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
    return -1;
  }
  return 0;
}
