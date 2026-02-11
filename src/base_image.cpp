#include "base_image.h"

#include <GLES2/gl2.h>

#include <algorithm>

#include "backend_GLSL.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

BaseTexture::BaseTexture() : _path(), _handle(0), _width(0), _height(0) {}

void BaseTexture::set_path(const std::string &path) {
	_path = path;
	unload();
}

const std::string &BaseTexture::path() const {
	return _path;
}

bool BaseTexture::is_loaded() const {
	return _handle != 0;
}

unsigned int BaseTexture::handle() const {
	return _handle;
}

bool BaseTexture::ensure_loaded() {
	if (_handle != 0) {
		return true;
	}
	if (_path.empty()) {
		return false;
	}

	int width = 0;
	int height = 0;
	int channels = 0;
	unsigned char *pixels = stbi_load(_path.c_str(), &width, &height, &channels, 4);
	if (!pixels) {
		return false;
	}

	GLSL::texture_to_gpu(_handle, pixels, width, height);
	stbi_image_free(pixels);

	_width = width;
	_height = height;
	return true;
}

void BaseTexture::unload() {
	if (_handle != 0) {
		glDeleteTextures(1, &_handle);
		_handle = 0;
	}
	_width = 0;
	_height = 0;
}

int BaseTexture::width() const {
	return _width;
}

int BaseTexture::height() const {
	return _height;
}

BaseTextureArray::BaseTextureArray() : _textures{}, _index(0) {}

void BaseTextureArray::clear() {
	for (auto &texture : _textures) {
		texture.set_path("");
	}
	_index = 0;
}

size_t BaseTextureArray::size() const {
	return _textures.size();
}

size_t BaseTextureArray::filled_count() const {
	size_t count = 0;
	for (const auto &texture : _textures) {
		if (!texture.path().empty()) {
			++count;
		}
	}
	return count;
}

void BaseTextureArray::set_path(size_t index, const std::string &value) {
	if (index >= _textures.size()) {
		return;
	}
	_textures[index].set_path(value);
}

const std::string &BaseTextureArray::path(size_t index) const {
	return _textures[index].path();
}

BaseTexture &BaseTextureArray::texture(size_t index) {
	return _textures[index];
}

const BaseTexture &BaseTextureArray::texture(size_t index) const {
	return _textures[index];
}

bool BaseTextureArray::select(size_t index) {
	if (index >= _textures.size()) {
		return false;
	}
	_index = index;
	return true;
}

size_t BaseTextureArray::index() const {
	return _index;
}

BaseTexture *BaseTextureArray::current() {
	if (_index >= _textures.size()) {
		return nullptr;
	}
	return &_textures[_index];
}

const BaseTexture *BaseTextureArray::current() const {
	if (_index >= _textures.size()) {
		return nullptr;
	}
	return &_textures[_index];
}

bool BaseTextureArray::ensure_current_loaded() {
	BaseTexture *active = current();
	if (!active) {
		return false;
	}
	return active->ensure_loaded();
}

void BaseTextureArray::next() {
	advance(1);
}

void BaseTextureArray::prev() {
	advance(-1);
}

void BaseTextureArray::advance(int step) {
	size_t next_index = find_next_index(_index, step);
	_index = next_index;
}

size_t BaseTextureArray::find_next_index(size_t start, int step) const {
	if (_textures.empty()) {
		return 0;
	}

	size_t count = _textures.size();
	size_t current = start % count;
	for (size_t offset = 1; offset <= count; ++offset) {
		size_t candidate = (current + count + (offset * step)) % count;
		if (!_textures[candidate].path().empty()) {
			return candidate;
		}
	}
	return start;
}
