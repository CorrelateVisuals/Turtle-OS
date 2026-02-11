#include "canvas.h"

Canvas::Canvas()
    : _source(CanvasSource::Color), _color{0.0f, 0.0f, 0.0f, 1.0f} {}

void Canvas::set_color(float r, float g, float b, float a) {
    _source = CanvasSource::Color;
    _color[0] = r;
    _color[1] = g;
    _color[2] = b;
    _color[3] = a;
}

void Canvas::set_texture_path(const std::string &path) {
    _source = CanvasSource::Texture;
    _path = path;
    _texture.set_path(path);
}

void Canvas::set_folder_path(const std::string &path) {
    _source = CanvasSource::Folder;
    _path = path;
    _gallery.clear();
    _finder.set_root(path);
    _finder.find_all_images(_gallery);
    _gallery.select(0);
}

CanvasSource Canvas::source() const {
    return _source;
}

const float *Canvas::color() const {
    return _color;
}

BaseTexture *Canvas::active_texture() {
    if (_source == CanvasSource::Texture) {
        return &_texture;
    }
    if (_source == CanvasSource::Folder) {
        return _gallery.current();
    }
    return nullptr;
}

const BaseTexture *Canvas::active_texture() const {
    if (_source == CanvasSource::Texture) {
        return &_texture;
    }
    if (_source == CanvasSource::Folder) {
        return _gallery.current();
    }
    return nullptr;
}

bool Canvas::ensure_active_loaded() {
    BaseTexture *texture = active_texture();
    if (!texture) {
        return false;
    }
    return texture->ensure_loaded();
}

void Canvas::next() {
    if (_source == CanvasSource::Folder) {
        _gallery.next();
    }
}

void Canvas::prev() {
    if (_source == CanvasSource::Folder) {
        _gallery.prev();
    }
}
