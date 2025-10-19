#pragma once
#include <string>
#include <utility>
#include "neural/Data_Handling/DataPoint.h"

// MnistLoader: read IDX (gz) MNIST files and convert to DataPoint**
class MnistLoader {
public:
    // Reads images and labels from gzipped IDX files and returns an array of DataPoint* and count.
    // Caller owns the returned DataPoint** and each DataPoint inside it.
    // Returns (nullptr,0) on error.
    static std::pair<DataPoint**, int> LoadFromGzFiles(const std::string& images_gz_path, const std::string& labels_gz_path, int num_labels = 10);
    // Convert IDX (gz or plain) files to the project's .bin dataset format
    // Returns true on success (and writes to out_bin_path), false otherwise.
    static bool ConvertToBin(const std::string& images_path, const std::string& labels_path, const std::string& out_bin_path, int num_labels = 10);
};
