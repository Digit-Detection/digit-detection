#pragma once
#include <vector>
#include "DataPoint.h"

namespace Augmentations {
    // Generate all augmentation combinations for a single small image (dst_w x dst_h)
    // The returned vector contains newly allocated DataPoint* which the caller must delete.
    std::vector<DataPoint*> GenerateAugmentedDataPoints(const double* src, int src_w, int src_h, int label, int num_labels);

    // Individual operations (operate src -> dst; dst must be allocated with dst_w*dst_h)
    void Scale(const double* src, int src_w, int src_h, double* dst, int dst_w, int dst_h, double factor);
    void Rotate(const double* src, int src_w, int src_h, double* dst, int dst_w, int dst_h, double degrees_clockwise);
    void ApplySaltAndPepper(const double* src, int src_w, int src_h, double* dst, int dst_w, int dst_h, double noise_fraction);
}
