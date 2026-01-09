
#include "image_finder.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <cstring>
#include <iostream>
#include <random>

namespace fs = std::filesystem;

ImageFinder::ImageFinder()
   : root("data") // default starting folder
{
   std::cout << "Initializing ImageFinder: " << std::endl;
}

void ImageFinder::set_root(const std::string &root_path)
{
   std::cout << "Setting root path: " << root_path << std::endl;
   root = root_path;
}

bool ImageFinder::is_image_file(const fs::path &p) const
{
   static const std::vector<std::string> extensions = {".jpg", ".jpeg", ".png",
                                                       ".bmp", ".gif",  ".webp"};

   std::string ext = p.extension().string();

   // Case-insensitive compare
   for (const auto &e : extensions) {
      if (strcasecmp(ext.c_str(), e.c_str()) == 0)
         return true;
   }
   std::cout << "No images found" << std::endl;
   return false;
}

void ImageFinder::find_all_images(std::array<std::string, TEXTURE_ARRAY_SIZE> &texture_array)
{
   size_t index = 0;

   for (const auto &entry : fs::recursive_directory_iterator(root)) {
      if (entry.is_directory() && entry.path().filename() == "images") {
         // Scan only inside the images/ folder
         for (const auto &Image : fs::directory_iterator(entry.path())) {
            if (Image.is_regular_file() && is_image_file(Image.path())) {
               if (index < TEXTURE_ARRAY_SIZE) {
                  texture_array[index++] = Image.path().string();
                  std::cout << index << '\n'
                            << Image.path().string() << std::endl
                            << std::endl;
               } else {
                  std::cout << "Texture array is full. Some images may not be added."
                            << std::endl;
                  return; // Exit the function if the array is full
               }
            }
         }
      }
   }

   std::cout << "Number of images found and added: " << index << std::endl;
}

template <size_t SIZE>
size_t ImageFinder::get_filled_array_count(const std::array<std::string, SIZE> &array)
{
   size_t count = 0;
   for (const auto &item : array) {
      if (!item.empty()) { // non-empty string indicates a filled entry
         ++count;
      }
   }
   return count;
}

void ImageFinder::randomize_order(std::array<std::string, TEXTURE_ARRAY_SIZE> &array)
{
   size_t filled_count = get_filled_array_count(array);
   if (filled_count > TEXTURE_ARRAY_SIZE) {
      std::cout << "Filled count exceeds array size!" << std::endl;
      return;
   }

   std::random_device rd;
   std::mt19937 gen(rd());

   // Shuffle only the filled part of the array
   std::shuffle(array.begin(), array.begin() + filled_count, gen);
}