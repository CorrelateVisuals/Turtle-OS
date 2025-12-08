#include <iostream>
#include <stdexcept>
#include "engine.h"

TurtleOS os;

int main() {
    try {
        os.run();

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
        return -1;
    }
    return 0;
}
