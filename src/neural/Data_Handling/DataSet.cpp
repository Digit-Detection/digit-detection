#include "neural/Data_Handling/DataSet.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>

void DataSet::SaveDataPoints(DataPoint** data, int data_length, const std::string& filename) {
    // Ensure parent directory exists
    try {
        std::filesystem::path p(filename);
        if (p.has_parent_path()) std::filesystem::create_directories(p.parent_path());
    } catch (...) {}
    std::ofstream file(filename, std::ios::binary | std::ios::trunc);
    if (!file) throw std::runtime_error("Failed to open dataset file for writing");
    for (int i = 0; i < data_length; ++i) {
        DataPoint* dp = data[i];
        int inputs_len = dp->get_inputs_length();
        int label = dp->get_label();
        int num_labels = dp->get_expected_outputs_length();

        file.write(reinterpret_cast<const char*>(&inputs_len), sizeof(int));
        file.write(reinterpret_cast<const char*>(&label), sizeof(int));
        file.write(reinterpret_cast<const char*>(&num_labels), sizeof(int));
        double* inputs = dp->get_inputs();
        if (inputs_len > 0) file.write(reinterpret_cast<const char*>(inputs), sizeof(double) * inputs_len);
    }
}

void DataSet::AppendDataPoint(DataPoint* dp, const std::string& filename) {
    try {
        std::filesystem::path p(filename);
        if (p.has_parent_path()) std::filesystem::create_directories(p.parent_path());
    } catch (...) {}
    std::ofstream file(filename, std::ios::binary | std::ios::out | std::ios::app);
    if (!file) throw std::runtime_error("Failed to open dataset file for appending");
    int inputs_len = dp->get_inputs_length();
    int label = dp->get_label();
    int num_labels = dp->get_expected_outputs_length();
    file.write(reinterpret_cast<const char*>(&inputs_len), sizeof(int));
    file.write(reinterpret_cast<const char*>(&label), sizeof(int));
    file.write(reinterpret_cast<const char*>(&num_labels), sizeof(int));
    double* inputs = dp->get_inputs();
    if (inputs_len > 0) file.write(reinterpret_cast<const char*>(inputs), sizeof(double) * inputs_len);
}

std::pair<DataPoint**, int> DataSet::LoadDataPoints(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Could not open dataset file: " << filename << std::endl;
        return std::make_pair((DataPoint**)nullptr, 0);
    }
    // Read sequentially until EOF
    std::vector<DataPoint*> items;
    while (file.peek() != EOF) {
        int inputs_len;
        int label;
        int num_labels;
        file.read(reinterpret_cast<char*>(&inputs_len), sizeof(int));
        if (!file) break;
        file.read(reinterpret_cast<char*>(&label), sizeof(int));
        file.read(reinterpret_cast<char*>(&num_labels), sizeof(int));
        double* inputs = nullptr;
        if (inputs_len > 0) {
            inputs = new double[inputs_len];
            file.read(reinterpret_cast<char*>(inputs), sizeof(double) * inputs_len);
        }
        DataPoint* dp = new DataPoint(inputs, inputs_len, label, num_labels);
        // DataPoint makes its own deep-copy of inputs; free the temporary buffer
        delete[] inputs;
        items.push_back(dp);
    }
    int n = (int)items.size();
    if (n == 0) return std::make_pair((DataPoint**)nullptr, 0);
    DataPoint** arr = new DataPoint*[n];
    for (int i = 0; i < n; ++i) arr[i] = items[i];
    return std::make_pair(arr, n);
}
