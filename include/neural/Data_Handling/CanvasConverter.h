#pragma once
#include "neural/Data_Handling/DataPoint.h"
#include <vector>

// Utilities to convert the GUI canvas grid into a DataPoint compatible with the network.
// The canvas grid is EXPECTED to be an array of size CANX * CANY (see constants.h).
class CanvasConverter {
public:
    // Convert a canvas grid (double array of 0/1) into a DataPoint.
    // If target_w/target_h > 0, the canvas will be resampled to (target_w x target_h)
    // using bilinear interpolation with proper pixel-center mapping. The function
    // returns a newly allocated DataPoint (caller owns it).
    static DataPoint* GridToDataPoint(const double* grid, int grid_w, int grid_h, int label, int num_labels, int target_w = -1, int target_h = -1);

    // Resample an input grid into a provided destination buffer using bilinear
    // interpolation. dst must point to at least dst_w * dst_h doubles. This
    // function does not allocate memory and is suitable for continuous updates.
    static void ResampleGrid(const double* src, int src_w, int src_h, double* dst, int dst_w, int dst_h);
};
