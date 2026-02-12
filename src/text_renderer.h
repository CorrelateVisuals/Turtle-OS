#pragma once

#include <GLES2/gl2.h>
#include <string>
#include <map>

class TextRenderer
{
public:
   TextRenderer();
   ~TextRenderer();

   void init(const std::string &font_path, int font_size);
   void render_text(const std::string &text, float x, float y, float scale, 
                    float r, float g, float b, float a);

   GLuint get_shader_program() const { return _shader_program; }

private:
   struct Character {
      GLuint texture_id;
      int width;
      int height;
      int bearing_x;
      int bearing_y;
      int advance;
   };

   void load_font(const std::string &font_path, int font_size);
   void create_text_shaders();

   std::map<char, Character> _characters;
   GLuint _vao;
   GLuint _vbo;
   GLuint _shader_program;
   GLint _projection_loc;
   GLint _text_color_loc;
};
