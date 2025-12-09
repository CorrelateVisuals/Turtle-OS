#pragma once

#include <string>
#include <vector>
#include <filesystem>

class ImageFinder {
public:
    ImageFinder();

    // Set root directory to scan (default: "data")
    void setRoot(const std::string& rootPath);

    // Perform recursive scan and return all image paths
    std::vector<std::string> findAllImages() const;

private:
    std::string root;

    bool isImageFile(const std::filesystem::path& p) const;
};