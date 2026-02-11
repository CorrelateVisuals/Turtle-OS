#include "shader.h"

#include <GLES2/gl2.h>

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

Shader::Shader() : _program(0) {}

Shader::~Shader() {
    if (_program != 0) {
        glDeleteProgram(_program);
    }
}

void Shader::load_from_files(const std::string &vertex_path,
                             const std::string &fragment_path) {
    std::string vertex_source = read_file(vertex_path);
    std::string fragment_source = read_file(fragment_path);

    unsigned int vertex_shader = compile(GL_VERTEX_SHADER, vertex_source);
    unsigned int fragment_shader = compile(GL_FRAGMENT_SHADER, fragment_source);

    _program = glCreateProgram();
    glAttachShader(_program, vertex_shader);
    glAttachShader(_program, fragment_shader);
    glLinkProgram(_program);

    int linked = 0;
    glGetProgramiv(_program, GL_LINK_STATUS, &linked);
    if (linked == 0) {
        int log_length = 0;
        glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &log_length);
        std::vector<char> log(static_cast<size_t>(log_length));
        glGetProgramInfoLog(_program, log_length, nullptr, log.data());
        std::string message(log.begin(), log.end());
        throw std::runtime_error("Shader link failed: " + message);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void Shader::use() const {
    glUseProgram(_program);
}

void Shader::set_uniform1f(const std::string &name, float value) const {
    int location = glGetUniformLocation(_program, name.c_str());
    if (location >= 0) {
        glUniform1f(location, value);
    }
}

void Shader::set_uniform1i(const std::string &name, int value) const {
    int location = glGetUniformLocation(_program, name.c_str());
    if (location >= 0) {
        glUniform1i(location, value);
    }
}

int Shader::get_attrib_location(const std::string &name) const {
    return glGetAttribLocation(_program, name.c_str());
}

std::string Shader::read_file(const std::string &path) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Failed to open shader file: " + path);
    }

    std::ostringstream contents;
    contents << file.rdbuf();
    return contents.str();
}

unsigned int Shader::compile(unsigned int type, const std::string &source) {
    unsigned int shader = glCreateShader(type);
    const char *source_ptr = source.c_str();
    glShaderSource(shader, 1, &source_ptr, nullptr);
    glCompileShader(shader);

    int compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == 0) {
        int log_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
        std::vector<char> log(static_cast<size_t>(log_length));
        glGetShaderInfoLog(shader, log_length, nullptr, log.data());
        std::string message(log.begin(), log.end());
        throw std::runtime_error("Shader compile failed: " + message);
    }

    return shader;
}
