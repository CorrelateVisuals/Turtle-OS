#include "app_time.h"

#include <GLFW/glfw3.h>

Time::Time() : _last(0.0), _delta(0.0f) {}

void Time::tick() {
    double now = glfwGetTime();
    _delta = static_cast<float>(now - _last);
    _last = now;
}

float Time::delta_seconds() const {
    return _delta;
}
