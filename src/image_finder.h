#pragma once

#include <array>
#include <filesystem>
#include <string>
#include <vector>

#include "base_image.h"

class ImageFinder
{
public:
   ImageFinder();

   template <size_t SIZE>
   size_t get_filled_array_count(const std::array<std::string, SIZE> &array);

   // Set root directory to scan (default: "data")
   void set_root(const std::string &root_path);

   // Return first image found (early exit)
   std::string find_first_image() const;

   // Perform recursive scan and return all image paths
   void find_all_images(BaseTextureArray &texture_array);

   void randomize_order(std::array<std::string, kTextureArraySize> &array);

private:
   std::string root;

   bool is_image_file(const std::filesystem::path &p) const;
};
