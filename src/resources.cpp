// #include "resources.h"
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

// GLuint load_texture(const std::string &path) {
//   stbi_set_flip_vertically_on_load(true);

//   int width, height, channels;
//   unsigned char *pixels =
//       stbi_load(path.c_str(), &width, &height, &channels, 4);

//   if (!pixels) {
//     fprintf(stderr, "Failed to load image: %s\n", path.c_str());
//     return 0;
//   }

//   GLuint texture;
//   glGenTextures(1, &texture);
//   glBindTexture(GL_TEXTURE_2D, texture);

//   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
//                GL_UNSIGNED_BYTE, pixels);

//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//   stbi_image_free(pixels);
//   return texture;
// }
