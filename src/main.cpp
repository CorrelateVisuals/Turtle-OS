#include <iostream>
#include <stdexcept>

#include "engine.h"
#include "image_finder.h"

int main(int argc, char *argv[]) {
  
  Engine turtle_os(argc, argv);

  try {
    turtle_os.run();

  } catch (const std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
    return -1;
  }
  return 0;
}
