#include <iostream>
#include <stdexcept>

#include "engine.h"
#include "geometry.h"

int main(int argc, char *argv[]) {
  std::string name1 = "new1";
  Geometry::Vertex tex1;
  Geometry::Vertex tex2{"1"};
  Geometry::Vertex tex3{{1.0f, 1.0f, 1.0f}, name1};


  Engine turtle_os(argc, argv);

  try {
    turtle_os.run();

  } catch (const std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
    return -1;
  }
  return 0;
}
