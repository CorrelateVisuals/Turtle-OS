#include "shader.h"
#include <stdio.h>
#include <stdlib.h>

char* load_shader_file(const char* path) {
    FILE* f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "Failed to open shader file: %s\n", path);
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    rewind(f);

    char* src = malloc(len + 1);
    fread(src, 1, len, f);
    src[len] = '\0';
    fclose(f);
    return src;
}

GLuint shader_compile(const char* source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        char buffer[512];
        glGetShaderInfoLog(shader, 512, NULL, buffer);
        fprintf(stderr, "Shader compile error: %s\n", buffer);
        return 0;
    }
    return shader;
}

GLuint shader_create_program(const char* vertex_src, const char* fragment_src) {
    GLuint vs = shader_compile(vertex_src, GL_VERTEX_SHADER);
    GLuint fs = shader_compile(fragment_src, GL_FRAGMENT_SHADER);

    GLuint prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glBindAttribLocation(prog, 0, "pos");
    glLinkProgram(prog);

    GLint status;
    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (!status) {
        char buffer[512];
        glGetProgramInfoLog(prog, 512, NULL, buffer);
        fprintf(stderr, "Program link error: %s\n", buffer);
        return 0;
    }
    glUseProgram(prog);
    return prog;
}
