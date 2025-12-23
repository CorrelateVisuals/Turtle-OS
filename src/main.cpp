#include <iostream>
#include <stdexcept>

#include "engine.h"
#include "image_finder.h"

Engine TurtleOS;

int process_arguments(int arg_count, char **argv) {
  for (int i = 1; i < arg_count; ++i) {
    std::string argument = argv[i];
    if (argument == "turtle"){
      std::cout << "turtle found!" << std::endl;
      return arg_count;
      }
  }
  return arg_count;
};

int main(int argc, char *argv[]) {

  process_arguments(argc, argv);

  try {
    TurtleOS.run();

  } catch (const std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
    return -1;
  }
  return 0;
}
