#include "neural/Data_Handling/CanvasConverter.h"
#include <cmath>
#include <iostream>
#include "constants.h"

// Helper: clamp value into [0,1]
// This ensures numeric resampling values are clamped to the valid input range
// used by the neural network (0 = background, 1 = foreground).
static inline double clamp01(double v) {
    if (v < 0.0) {
        return 0.0;
    }
    if (v > 1.0) {
        return 1.0;
    }
    return v;
}

// Bilinear sample at floating coords (x, y) in source grid
static double bilinear_sample(const double* src, int src_w, int src_h, double x, double y) {
    if (src_w <= 0 || src_h <= 0) {
        return 0.0;
    }
    // clamp coordinates to [0, w-1] x [0, h-1]
    if (x < 0.0) {
        x = 0.0;
    }
    if (x > src_w - 1) {
        x = src_w - 1;
    }
    if (y < 0.0) {
        y = 0.0;
    }
    if (y > src_h - 1) {
        y = src_h - 1;
    }

    int x0 = (int)std::floor(x);
    int x1 = x0 + 1;
    int y0 = (int)std::floor(y);
    int y1 = y0 + 1;

    if (x1 >= src_w) {
        x1 = src_w - 1;
    }
    if (y1 >= src_h) {
        y1 = src_h - 1;
    }

    double dx = x - x0;
    double dy = y - y0;

    double v00 = src[y0 * src_w + x0];
    double v10 = src[y0 * src_w + x1];
    double v01 = src[y1 * src_w + x0];
    double v11 = src[y1 * src_w + x1];

    double v0 = v00 * (1 - dx) + v10 * dx;
    double v1 = v01 * (1 - dx) + v11 * dx;
    double v = v0 * (1 - dy) + v1 * dy;
    return v;
}
void CanvasConverter::ResampleGrid(const double* src, int src_w, int src_h, double* dst, int dst_w, int dst_h) {
    if (!src || !dst || src_w <= 0 || src_h <= 0 || dst_w <= 0 || dst_h <= 0) {
        return;
    }
    double scale_x = static_cast<double>(src_w) / static_cast<double>(dst_w);
    double scale_y = static_cast<double>(src_h) / static_cast<double>(dst_h);
    for (int y = 0; y < dst_h; ++y) {
        for (int x = 0; x < dst_w; ++x) {
            double src_x = (x + 0.5) * scale_x - 0.5;
            double src_y = (y + 0.5) * scale_y - 0.5;
            double val = bilinear_sample(src, src_w, src_h, src_x, src_y);
            dst[y * dst_w + x] = clamp01(val);
        }
    }
}

DataPoint* CanvasConverter::GridToDataPoint(const double* grid, int grid_w, int grid_h, int label, int num_labels, int target_w, int target_h) {
    if (grid == nullptr) {
        return nullptr;
    }

    // If no target specified, use constants
    if (target_w <= 0) {
        target_w = CONSTANTS_H::DESTX;
    }
    if (target_h <= 0) {
        target_h = CONSTANTS_H::DESTY;
    }

    int out_w = target_w;
    int out_h = target_h;
    double* out = new double[out_w * out_h];
    ResampleGrid(grid, grid_w, grid_h, out, out_w, out_h);
    DataPoint* dp = new DataPoint(out, out_w * out_h, label, num_labels);
    delete[] out;
    return dp;
}
