#ifndef TURTLE_OS_CANVAS_H
#define TURTLE_OS_CANVAS_H

#include <string>

#include "base_image.h"
#include "image_finder.h"

enum class CanvasSource {
    Color,
    Texture,
    Folder
};

class Canvas {
public:
    Canvas();

    void set_color(float r, float g, float b, float a);
    void set_texture_path(const std::string &path);
    void set_folder_path(const std::string &path);

    CanvasSource source() const;
    const float *color() const;
    BaseTexture *active_texture();
    const BaseTexture *active_texture() const;
    bool ensure_active_loaded();
    void next();
    void prev();

private:
    CanvasSource _source;
    float _color[4];
    std::string _path;
    BaseTexture _texture;
    BaseTextureArray _gallery;
    ImageFinder _finder;
};

#endif // TURTLE_OS_CANVAS_H
