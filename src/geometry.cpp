#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "geometry.h"

Geometry::Vertex::Vertex(std::array<float, 3> pos, std::array<float, 3> rot,
                         std::string name)
    : _position(pos), _rotation(rot), _name(name) {
  print_config();
}

void Geometry::Vertex::print_config() {
  std::cout << _name << " created at position and rotation: " << std::endl
            << "    "
            << "x: " << _position[0] << " y: " << _position[1]
            << " z: " << _position[2] << std::endl
            << "    "
            << "x: " << _rotation[0] << " y: " << _rotation[1]
            << " z: " << _rotation[2] << std::endl
            << std::endl;
}

Geometry::Shape::Shape(int shape, float scale, std::array<float, 3> pos,
                       std::array<float, 3> rot, std::string name)
    : Vertex(pos, rot, name), _scale(scale), _shape(shape) {
  print_config();
}

void Geometry::Shape::print_config() {
  std::cout << "Shape number type: " << _shape << " size of " << _scale
            << std::endl
            << std::endl;
}

void Img::Texture::print_config() {
  std::cout << "Texture size: " << _images.size() << " , index " << _image_index
            << _current_image << std::endl
            << std::endl;
}

void Img::Texture::select_texture(int index) {
  if (_images.empty())
    return;

  _image_index = (index + _images.size()) % _images.size();

  std::cout << "Switching to texture: " << _images[_image_index] << "\n";

  _zoom.reset_start_scale();

  _current_image = load_texture(_images[_image_index]);
  glBindTexture(GL_TEXTURE_2D, _current_image);

  return;
}

GLuint Img::Texture::load_texture(const std::string &path) {
  stbi_set_flip_vertically_on_load(true);

  int width, height, channels;
  unsigned char *pixels =
      stbi_load(path.c_str(), &width, &height, &channels, 4);

  if (!pixels) {
    fprintf(stderr, "Failed to load image: %s\n", path.c_str());
    return 0;
  }

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, pixels);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_image_free(pixels);
  return texture;
}

Effects::ScaleVertices::ScaleVertices(float start, float speed, float final)
    : _reset_start_scale{start}, _texture_start_scale{start},
      _zoom_speed(speed), _texture_final_scale(final + speed) {
  std::cout << "Initializing Zoom." << std::endl;
  getSettings();
}

void Effects::ScaleVertices::run(bool is_on) {
  if (is_on) {
    if (_texture_start_scale > _texture_final_scale) {
      _texture_start_scale -= _zoom_speed;
    } else {
      _texture_start_scale = _texture_final_scale;
    }
  }
}

void Effects::ScaleVertices::reset_start_scale() {
  // std::cout << _texture_start_scale << _reset_start_scale << std::endl;
  _texture_start_scale = _reset_start_scale;
}

void Effects::ScaleVertices::getSettings() {
  std::cout << "Zoom _scale_location: " << _scale_location
            << ",  _texture_start_scale: " << _texture_start_scale
            << ", _zoom_speed: " << _zoom_speed
            << ", _texture_final_scale:" << _texture_final_scale << std::endl
            << std::endl;
}
