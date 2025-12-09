#include <algorithm>
#include <cctype>
#include <cstring>

#include "image_finder.h"

namespace fs = std::filesystem;

ImageFinder::ImageFinder()
    : root("data") // default starting folder
{
}

void ImageFinder::setRoot(const std::string& rootPath) {
    root = rootPath;
}

bool ImageFinder::isImageFile(const fs::path& p) const {
    static const std::vector<std::string> extensions = {
        ".jpg", ".jpeg", ".png", ".bmp", ".gif", ".webp"
    };

    std::string ext = p.extension().string();

    // Case-insensitive compare
    for (const auto& e : extensions) {
        if (strcasecmp(ext.c_str(), e.c_str()) == 0)
            return true;
    }
    return false;
}

std::vector<std::string> ImageFinder::findAllImages() const {
    std::vector<std::string> result;

    for (const auto& entry : fs::recursive_directory_iterator(root)) {
        if (entry.is_directory() && entry.path().filename() == "images") {

            // Scan only inside the images/ folder
            for (const auto& img : fs::directory_iterator(entry.path())) {
                if (img.is_regular_file() && isImageFile(img.path())) {
                    result.push_back(img.path().string());
                }
            }
        }
    }

    return result;
}
