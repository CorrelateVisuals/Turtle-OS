#ifndef SHADER_H
#define SHADER_H

#include <GLES2/gl2.h>

char* load_shader_file(const char* path);
GLuint shader_compile(const char* source, GLenum type);
GLuint shader_create_program(const char* vertex_src, const char* fragment_src);

#endif
