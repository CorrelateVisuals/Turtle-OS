#ifndef TURTLE_OS_BASE_SHADER_ACCESS_H
#define TURTLE_OS_BASE_SHADER_ACCESS_H

class BaseUniform {
public:
	BaseUniform();
	BaseUniform(int location, float value);

	int location() const;
	float value() const;
	void set_value(float value);

private:
	int _location;
	float _value;
};

#endif // TURTLE_OS_BASE_SHADER_ACCESS_H
