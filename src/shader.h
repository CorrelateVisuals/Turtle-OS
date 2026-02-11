#pragma once

#include "scale_vertices.h"

#include <GLES2/gl2.h>

#include <string>

// cppcheck-suppress syntaxError
[[nodiscard]] std::string load_shader_file(const std::string &path);
GLuint shader_compile(const std::string &source, GLenum type);
GLuint shader_create_program(const std::string &vertex_src, const std::string &fragment_src);

void create_shaders(turtle::ScaleVertices &zoom);
