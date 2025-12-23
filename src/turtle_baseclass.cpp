#include "turtle_baseclass.h"
#include "resources.h"
#include "shader.h"

TurtleBase::TurtleBase(int argc, char *argv[]) {
  for (int i = 1; i < argc; ++i) {
    std::string argument = argv[i];
    if (argument == "turtle") {
      std::cout << "TurtleBase found!\n" << std::endl;
      return;
    }
  }
  return;
};

TurtleBase::Image::Zoom::Zoom(float start, float speed, float final)
    : _reset_start_scale{start}, _texture_start_scale{start},
      _zoom_speed(speed), _texture_final_scale(final + speed) {
  std::cout << "Initializing Zoom." << std::endl;
  getSettings();
};

void TurtleBase::Image::Zoom::run(bool is_on) {

  if (is_on) {
    if (_texture_start_scale > _texture_final_scale) {
      _texture_start_scale -= _zoom_speed;
    } else {
      _texture_start_scale = _texture_final_scale;
    }
  }
};

void TurtleBase::Image::Zoom::getSettings() {
  std::cout << "Zoom _scale_location: " << _scale_location
            << ",  _texture_start_scale: " << _texture_start_scale
            << ", _zoom_speed: " << _zoom_speed
            << ", _texture_final_scale:" << _texture_final_scale << std::endl
            << std::endl;
};

void TurtleBase::Image::Texture::select_texture(int index, Zoom &effect) {
  if (_images.empty())
    return;

  _image_index = (index + _images.size()) % _images.size();

  std::cout << "Switching to texture: " << _images[_image_index] << "\n";

  effect.reset_start_scale();

  _current_image = load_texture(_images[_image_index]);
  glBindTexture(GL_TEXTURE_2D, _current_image);

  return;
}

void TurtleBase::Shaders::create_shaders(GLint& scale_location) {
  // Load shader sources from files
  const std::string vert_src = load_shader_file("src/shaders/vertex.glsl");
  const std::string frag_src = load_shader_file("src/shaders/fragment.glsl");

  // Compile and link shader program
  const GLuint shaderProgram = shader_create_program(vert_src, frag_src);
  if (!shaderProgram) {
    throw std::runtime_error("Failed to create shader program");
  }

  glUseProgram(shaderProgram);

  scale_location = glGetUniformLocation(shaderProgram, "scale");
}

