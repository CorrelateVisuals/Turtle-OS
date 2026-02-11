#ifndef TURTLE_OS_SHADER_H
#define TURTLE_OS_SHADER_H

#include <string>

class Shader {
public:
    Shader();
    ~Shader();

    void load_from_files(const std::string &vertex_path,
                         const std::string &fragment_path);
    void use() const;

    void set_uniform1f(const std::string &name, float value) const;
    void set_uniform1i(const std::string &name, int value) const;
    int get_attrib_location(const std::string &name) const;

private:
    unsigned int _program;

    static std::string read_file(const std::string &path);
    static unsigned int compile(unsigned int type, const std::string &source);
};

#endif // TURTLE_OS_SHADER_H
