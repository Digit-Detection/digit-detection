#include "neural/Data_Handling/CanvasConverter.h"
#include <cmath>
#include <iostream>

DataPoint* CanvasConverter::GridToDataPoint(const double* grid, int grid_w, int grid_h, int label, int num_labels, int target_size) {
    if (grid == nullptr) return nullptr;
    if (target_size <= 0) target_size = grid_w; // no resize

    // If resizing is required, perform nearest-neighbour down/up-sampling to target_size x target_size
    int out_w = target_size;
    int out_h = target_size;
    double* out = new double[out_w * out_h];

    for (int y = 0; y < out_h; ++y) {
        for (int x = 0; x < out_w; ++x) {
            // Map output pixel to input pixel with simple scaling
            int src_x = std::round((x + 0.0) * (grid_w - 1) / (out_w - 1));
            int src_y = std::round((y + 0.0) * (grid_h - 1) / (out_h - 1));
            if (src_x < 0) src_x = 0; if (src_x >= grid_w) src_x = grid_w - 1;
            if (src_y < 0) src_y = 0; if (src_y >= grid_h) src_y = grid_h - 1;
            double val = grid[src_y * grid_w + src_x];
            // normalize (grid already 0 or 1) but keep values as double between 0-1
            out[y * out_w + x] = val;
        }
    }

    // Create DataPoint (DataPoint will deep copy inputs)
    DataPoint* dp = new DataPoint(out, out_w * out_h, label, num_labels);
    delete[] out;
    return dp;
}
