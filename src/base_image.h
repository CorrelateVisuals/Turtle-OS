#ifndef TURTLE_OS_BASE_IMAGE_H
#define TURTLE_OS_BASE_IMAGE_H

#include <array>
#include <cstddef>
#include <string>

constexpr size_t kTextureArraySize = 1000;

class BaseTexture {
public:
	BaseTexture();

	void set_path(const std::string &path);
	const std::string &path() const;

	bool is_loaded() const;
	unsigned int handle() const;
	bool ensure_loaded();
	void unload();

	int width() const;
	int height() const;

private:
	std::string _path;
	unsigned int _handle;
	int _width;
	int _height;
};

class BaseTextureArray {
public:
	BaseTextureArray();

	void clear();
	size_t size() const;
	size_t filled_count() const;

	void set_path(size_t index, const std::string &value);
	const std::string &path(size_t index) const;

	BaseTexture &texture(size_t index);
	const BaseTexture &texture(size_t index) const;

	bool select(size_t index);
	size_t index() const;
	BaseTexture *current();
	const BaseTexture *current() const;
	bool ensure_current_loaded();

	void next();
	void prev();

private:
	void advance(int step);
	size_t find_next_index(size_t start, int step) const;

	std::array<BaseTexture, kTextureArraySize> _textures;
	size_t _index;
};

#endif // TURTLE_OS_BASE_IMAGE_H
