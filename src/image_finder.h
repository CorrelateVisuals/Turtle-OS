#pragma once

#include <filesystem>
#include <string>
#include <vector>

constexpr int TEXTURE_ARRAY_SIZE = 1000;

class ImageFinder
{
public:
  ImageFinder();

  // Set root directory to scan (default: "data")
  void set_root(const std::string &root_path);

  // Perform recursive scan and return all image paths
  void find_all_images(std::array<std::string, TEXTURE_ARRAY_SIZE> &texture_array);

  void randomize_order(std::array<std::string, TEXTURE_ARRAY_SIZE> &array);

private:
  std::string root;

  bool is_image_file(const std::filesystem::path &p) const;
};