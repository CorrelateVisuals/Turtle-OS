#pragma once

#include <string>
#include <vector>
#include <filesystem>


class ImageFinder {
public:
    ImageFinder();

    // Set root directory to scan (default: "data")
    void set_root(const std::string& root_path);

    // Perform recursive scan and return all image paths
    std::vector<std::string> find_all_images() const;

    void randomize_order(std::vector<std::string>& vector);    

private:
    std::string root;

    bool is_image_file(const std::filesystem::path& p) const;
};