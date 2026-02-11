#include "base_shader_access.h"

BaseUniform::BaseUniform() : _location(-1), _value(0.0f) {}

BaseUniform::BaseUniform(int location, float value)
	: _location(location), _value(value) {}

int BaseUniform::location() const {
	return _location;
}

float BaseUniform::value() const {
	return _value;
}

void BaseUniform::set_value(float value) {
	_value = value;
}
