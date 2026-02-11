#include "shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

std::string load_shader_file(const std::string &path)
{
   std::ifstream file(path);
   if (!file.is_open()) {
      throw std::runtime_error("Failed to open shader file: " + path);
   }

   std::ostringstream ss;
   ss << file.rdbuf();
   return ss.str();
}

GLuint shader_compile(const std::string &source, GLenum type)
{
   GLuint shader = glCreateShader(type);
   const char *src = source.c_str();
   glShaderSource(shader, 1, &src, nullptr);
   glCompileShader(shader);

   GLint status;
   glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
   if (!status) {
      char buffer[512];
      glGetShaderInfoLog(shader, 512, nullptr, buffer);
      std::cerr << "Shader compile error: " << buffer << "\n";
      return 0;
   }

   return shader;
}

GLuint shader_create_program(const std::string &vertex_src, const std::string &fragment_src)
{
   GLuint vs = shader_compile(vertex_src, GL_VERTEX_SHADER);
   if (!vs)
      return 0;
   GLuint fs = shader_compile(fragment_src, GL_FRAGMENT_SHADER);
   if (!fs)
      return 0;

   GLuint prog = glCreateProgram();
   glAttachShader(prog, vs);
   glAttachShader(prog, fs);
   glBindAttribLocation(prog, 0, "pos");
   glLinkProgram(prog);

   GLint status;
   glGetProgramiv(prog, GL_LINK_STATUS, &status);
   if (!status) {
      char buffer[512];
      glGetProgramInfoLog(prog, 512, nullptr, buffer);
      std::cerr << "Program link error: " << buffer << "\n";
      return 0;
   }

   glDeleteShader(vs);
   glDeleteShader(fs);

   return prog;
}

void create_shaders(TurtleBase::ScaleVertices &zoom)
{
   // Load shader sources from files
   const std::string vert_src = load_shader_file("src/shaders/vertex.glsl");
   const std::string frag_src = load_shader_file("src/shaders/fragment.glsl");

   // Compile and link shader program
   const GLuint shaderProgram = shader_create_program(vert_src, frag_src);
   if (!shaderProgram) {
      throw std::runtime_error("Failed to create shader program");
   }

   glUseProgram(shaderProgram);

   zoom.set_shader_location(glGetUniformLocation(shaderProgram, "scale"));
   TurtleBase::Uniform::register_location_value("scale", zoom.get_shader_location());
}