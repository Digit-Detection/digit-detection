#include "neural/Data_Handling/MnistLoader.h"
#include <zlib.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <algorithm>
#include "neural/Data_Handling/DataSet.h"

// Read big-endian 32-bit int from 4 bytes
static int read_be_int(const unsigned char* buf) {
    return (int)((buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3]);
}

std::pair<DataPoint**, int> MnistLoader::LoadFromGzFiles(const std::string& images_gz_path, const std::string& labels_gz_path, int num_labels) {
    bool images_gz = false;
    bool labels_gz = false;
    if (images_gz_path.size() > 3 && images_gz_path.substr(images_gz_path.size()-3) == ".gz") images_gz = true;
    if (labels_gz_path.size() > 3 && labels_gz_path.substr(labels_gz_path.size()-3) == ".gz") labels_gz = true;

    // We'll provide two read paths: gzipped via zlib, or plain ifstream.
    unsigned char header[16];
    int magic_images = 0, num_images = 0, rows = 0, cols = 0;
    if (images_gz) {
        gzFile imgf = gzopen(images_gz_path.c_str(), "rb");
        if (!imgf) {
            std::cerr << "Failed to open images file: " << images_gz_path << std::endl;
            return std::make_pair((DataPoint**)nullptr, 0);
        }
        if (gzread(imgf, header, 16) != 16) {
            std::cerr << "Failed to read image header from: " << images_gz_path << std::endl;
            gzclose(imgf); return std::make_pair((DataPoint**)nullptr, 0);
        }
        magic_images = read_be_int(header);
        num_images = read_be_int(header + 4);
        rows = read_be_int(header + 8);
        cols = read_be_int(header + 12);
        // close header-only handle; we'll reopen for full reads below
        gzclose(imgf);
    } else {
        std::ifstream ifs(images_gz_path, std::ios::binary);
        if (!ifs) {
            std::cerr << "Failed to open images file: " << images_gz_path << std::endl;
            return std::make_pair((DataPoint**)nullptr, 0);
        }
        ifs.read(reinterpret_cast<char*>(header), 16);
        if (!ifs) {
            std::cerr << "Failed to read image header from: " << images_gz_path << std::endl;
            return std::make_pair((DataPoint**)nullptr, 0);
        }
        magic_images = read_be_int(header);
        num_images = read_be_int(header + 4);
        rows = read_be_int(header + 8);
        cols = read_be_int(header + 12);
        ifs.close();
    }
    if (magic_images != 0x00000803) {
        std::cerr << "Unexpected image file magic: 0x" << std::hex << magic_images << std::dec << std::endl;
        return std::make_pair((DataPoint**)nullptr, 0);
    }

    // Read label header: magic(4), count(4)
    unsigned char labheader[8];
    int magic_labels = 0, num_labels_file = 0;
    if (labels_gz) {
        gzFile labf = gzopen(labels_gz_path.c_str(), "rb");
        if (!labf) {
            std::cerr << "Failed to open labels file: " << labels_gz_path << std::endl;
            return std::make_pair((DataPoint**)nullptr, 0);
        }
        if (gzread(labf, labheader, 8) != 8) {
            std::cerr << "Failed to read label header from: " << labels_gz_path << std::endl;
            gzclose(labf); return std::make_pair((DataPoint**)nullptr, 0);
        }
        magic_labels = read_be_int(labheader);
        num_labels_file = read_be_int(labheader + 4);
        gzclose(labf);
    } else {
        std::ifstream lfs(labels_gz_path, std::ios::binary);
        if (!lfs) {
            std::cerr << "Failed to open labels file: " << labels_gz_path << std::endl;
            return std::make_pair((DataPoint**)nullptr, 0);
        }
        lfs.read(reinterpret_cast<char*>(labheader), 8);
        if (!lfs) {
            std::cerr << "Failed to read label header from: " << labels_gz_path << std::endl;
            return std::make_pair((DataPoint**)nullptr, 0);
        }
        magic_labels = read_be_int(labheader);
        num_labels_file = read_be_int(labheader + 4);
        lfs.close();
    }

    int n = std::min(num_images, num_labels_file);
    int pixels = rows * cols;
    std::vector<DataPoint*> items;
    items.reserve(n);

    unsigned char* imgbuf = new unsigned char[pixels];
    unsigned char labbuf[1];

    // Unified reading: if gz, use gzread and reopen gzFile; else use ifstream
    if (images_gz && labels_gz) {
        gzFile imgf2 = gzopen(images_gz_path.c_str(), "rb");
        gzFile labf2 = gzopen(labels_gz_path.c_str(), "rb");
        // skip headers
        gzread(imgf2, header, 16);
        gzread(labf2, labheader, 8);
        for (int i = 0; i < n; ++i) {
            int r = gzread(imgf2, imgbuf, pixels);
            if (r != pixels) { std::cerr << "Failed to read image " << i << "\n"; break; }
            int lr = gzread(labf2, labbuf, 1);
            if (lr != 1) { std::cerr << "Failed to read label " << i << "\n"; break; }
            double* inputs = new double[pixels];
            for (int p = 0; p < pixels; ++p) inputs[p] = imgbuf[p] / 255.0;
            int label = (int)labbuf[0];
            DataPoint* dp = new DataPoint(inputs, pixels, label, num_labels);
            delete[] inputs;
            items.push_back(dp);
        }
        gzclose(imgf2); gzclose(labf2);
    } else if (!images_gz && !labels_gz) {
        std::ifstream ifs2(images_gz_path, std::ios::binary);
        std::ifstream lfs2(labels_gz_path, std::ios::binary);
        ifs2.seekg(16, std::ios::beg);
        lfs2.seekg(8, std::ios::beg);
        for (int i = 0; i < n; ++i) {
            ifs2.read(reinterpret_cast<char*>(imgbuf), pixels);
            if (!ifs2) { std::cerr << "Failed to read image " << i << "\n"; break; }
            lfs2.read(reinterpret_cast<char*>(labbuf), 1);
            if (!lfs2) { std::cerr << "Failed to read label " << i << "\n"; break; }
            double* inputs = new double[pixels];
            for (int p = 0; p < pixels; ++p) inputs[p] = imgbuf[p] / 255.0;
            int label = (int)labbuf[0];
            DataPoint* dp = new DataPoint(inputs, pixels, label, num_labels);
            delete[] inputs;
            items.push_back(dp);
        }
        ifs2.close(); lfs2.close();
    } else {
        std::cerr << "Mixed compressed/uncompressed MNIST files are not supported. Provide both gz or both plain." << std::endl;
    }

    int m = (int)items.size();
    if (m == 0) return std::make_pair((DataPoint**)nullptr, 0);
    DataPoint** arr = new DataPoint*[m];
    for (int i = 0; i < m; ++i) arr[i] = items[i];
    return std::make_pair(arr, m);
}

bool MnistLoader::ConvertToBin(const std::string& images_path, const std::string& labels_path, const std::string& out_bin_path, int num_labels) {
    auto res = MnistLoader::LoadFromGzFiles(images_path, labels_path, num_labels);
    if (res.second == 0 || res.first == nullptr) return false;
    // Save to .bin format using DataSet
    try {
        DataSet::SaveDataPoints(res.first, res.second, out_bin_path);
    } catch (const std::exception& ex) {
        std::cerr << "Failed to save mnist to bin: " << ex.what() << std::endl;
        // cleanup
        for (int i = 0; i < res.second; ++i) delete res.first[i];
        delete [] res.first;
        return false;
    }
    // cleanup
    for (int i = 0; i < res.second; ++i) delete res.first[i];
    delete [] res.first;
    return true;
}
