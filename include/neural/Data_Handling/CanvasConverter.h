#pragma once
#include "neural/Data_Handling/DataPoint.h"
#include <vector>

// Utilities to convert the GUI canvas grid into a DataPoint compatible with the network.
// The canvas grid is EXPECTED to be an array of size CANX * CANY (see constants.h).
class CanvasConverter {
public:
    // Convert a canvas grid (double array of 0/1) into a DataPoint.
    // If target_size > 0, the canvas will be resampled to (target_size x target_size) using nearest-neighbour.
    // Returns a newly allocated DataPoint (caller owns it).
    static DataPoint* GridToDataPoint(const double* grid, int grid_w, int grid_h, int label, int num_labels, int target_size = 28);
};
