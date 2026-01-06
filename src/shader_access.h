


class ShaderAccess
{
public:

    class Uniform
    {
    public:
        Uniform(float v, float l) : value(v), location(l) {};
        ~Uniform() = default;

        float value;
        int location;
    };
};