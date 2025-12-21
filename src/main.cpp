#include <iostream>
#include <stdexcept>

#include "engine.h"
#include "image_finder.h"

Engine TurtleOS;

int main() {

  try {
    TurtleOS.run();

  } catch (const std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
    return -1;
  }
  return 0;
}
