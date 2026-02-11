#include <exception>
#include <iostream>

#include "engine.h"

int main() {
    try {
        Engine engine;
        engine.run();
    } catch (const std::exception &ex) {
        std::cerr << "Fatal: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
