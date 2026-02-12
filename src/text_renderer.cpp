#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include "text_renderer.h"
#include <iostream>
#include <fstream>
#include <vector>

TextRenderer::TextRenderer()
   : _vao(0), _vbo(0), _shader_program(0), _projection_loc(-1), _text_color_loc(-1)
{
}

TextRenderer::~TextRenderer()
{
   for (auto &pair : _characters) {
      if (pair.second.texture_id) {
         glDeleteTextures(1, &pair.second.texture_id);
      }
   }
}

void TextRenderer::init(const std::string &font_path, int font_size)
{
   create_text_shaders();
   load_font(font_path, font_size);
   
   // Create VBO for rendering quads
   glGenBuffers(1, &_vbo);
}

void TextRenderer::create_text_shaders()
{
   const char *vertex_shader_source = R"(
      attribute vec4 vertex; // <vec2 pos, vec2 tex>
      varying vec2 TexCoords;
      
      void main()
      {
         gl_Position = vec4(vertex.xy, 0.0, 1.0);
         TexCoords = vertex.zw;
      }
   )";

   const char *fragment_shader_source = R"(
      precision mediump float;
      varying vec2 TexCoords;
      uniform sampler2D text;
      uniform vec4 textColor;
      
      void main()
      {
         vec4 sampled = vec4(1.0, 1.0, 1.0, texture2D(text, TexCoords).r);
         gl_FragColor = textColor * sampled;
      }
   )";

   // Compile vertex shader
   GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
   glCompileShader(vertex_shader);

   GLint success;
   glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
   if (!success) {
      char info_log[512];
      glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
      std::cerr << "ERROR: Vertex shader compilation failed\n" << info_log << std::endl;
   }

   // Compile fragment shader
   GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
   glCompileShader(fragment_shader);

   glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
   if (!success) {
      char info_log[512];
      glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
      std::cerr << "ERROR: Fragment shader compilation failed\n" << info_log << std::endl;
   }

   // Link shaders
   _shader_program = glCreateProgram();
   glAttachShader(_shader_program, vertex_shader);
   glAttachShader(_shader_program, fragment_shader);
   glLinkProgram(_shader_program);

   glGetProgramiv(_shader_program, GL_LINK_STATUS, &success);
   if (!success) {
      char info_log[512];
      glGetProgramInfoLog(_shader_program, 512, NULL, info_log);
      std::cerr << "ERROR: Shader program linking failed\n" << info_log << std::endl;
   }

   glDeleteShader(vertex_shader);
   glDeleteShader(fragment_shader);

   _text_color_loc = glGetUniformLocation(_shader_program, "textColor");
}

void TextRenderer::load_font(const std::string &font_path, int font_size)
{
   // Read font file
   std::ifstream file(font_path, std::ios::binary | std::ios::ate);
   if (!file.is_open()) {
      std::cerr << "ERROR: Failed to open font file: " << font_path << std::endl;
      return;
   }

   std::streamsize size = file.tellg();
   file.seekg(0, std::ios::beg);

   std::vector<unsigned char> buffer(size);
   if (!file.read((char*)buffer.data(), size)) {
      std::cerr << "ERROR: Failed to read font file" << std::endl;
      return;
   }

   // Initialize stb_truetype
   stbtt_fontinfo font;
   if (!stbtt_InitFont(&font, buffer.data(), 0)) {
      std::cerr << "ERROR: Failed to initialize font" << std::endl;
      return;
   }

   float scale = stbtt_ScaleForPixelHeight(&font, font_size);

   // Load first 128 ASCII characters
   for (unsigned char c = 0; c < 128; c++) {
      int width, height, xoff, yoff;
      unsigned char *bitmap = stbtt_GetCodepointBitmap(&font, 0, scale, c, 
                                                        &width, &height, &xoff, &yoff);

      // Create texture
      GLuint texture;
      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, 
                   GL_ALPHA, GL_UNSIGNED_BYTE, bitmap);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      int advance, lsb;
      stbtt_GetCodepointHMetrics(&font, c, &advance, &lsb);

      Character character = {
         texture,
         width,
         height,
         xoff,
         yoff,
         (int)(advance * scale)
      };

      _characters[c] = character;
      stbtt_FreeBitmap(bitmap, NULL);
   }
}

void TextRenderer::render_text(const std::string &text, float x, float y, float scale,
                                float r, float g, float b, float a)
{
   glUseProgram(_shader_program);
   glUniform4f(_text_color_loc, r, g, b, a);
   
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   float current_x = x;

   for (auto c : text) {
      if (_characters.find(c) == _characters.end()) {
         continue;
      }

      Character ch = _characters[c];

      float xpos = current_x + ch.bearing_x * scale;
      float ypos = y - (ch.height - ch.bearing_y) * scale;

      float w = ch.width * scale;
      float h = ch.height * scale;

      // Update VBO for each character
      float vertices[6][4] = {
         { xpos,     ypos + h, 0.0f, 0.0f },
         { xpos,     ypos,     0.0f, 1.0f },
         { xpos + w, ypos,     1.0f, 1.0f },

         { xpos,     ypos + h, 0.0f, 0.0f },
         { xpos + w, ypos,     1.0f, 1.0f },
         { xpos + w, ypos + h, 1.0f, 0.0f }
      };

      glBindTexture(GL_TEXTURE_2D, ch.texture_id);
      glBindBuffer(GL_ARRAY_BUFFER, _vbo);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

      glDrawArrays(GL_TRIANGLES, 0, 6);

      // Advance cursor for next glyph
      current_x += ch.advance * scale;
   }

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindTexture(GL_TEXTURE_2D, 0);
   glDisable(GL_BLEND);
}
