#include <iostream>
#include <stdexcept>

#include "engine.h"
#include "image_finder.h"

Engine TurtleOS;

int main() {
    ImageFinder finder;

    finder.setRoot("data");  // optional, default is "data/"

    auto images = finder.findAllImages();

    for (const auto& path : images) {
        std::cout << path << "\n";
    }
    try {
        TurtleOS.run();

    

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
        return -1;
    }
    return 0;
}
