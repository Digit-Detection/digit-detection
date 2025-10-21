#include "neural/Data_Handling/Augmentations.h"
#include "constants.h"
#include <cmath>
#include <random>
#include <iostream>

namespace Augmentations {

// Helper: bilinear sample with pad-to-zero outside
static inline double sample_bilinear_pad(const double* src, int w, int h, double x, double y) {
    // If completely outside, return 0
    if (x < 0.0 || y < 0.0 || x > w - 1 || y > h - 1) return 0.0;
    int x0 = (int)std::floor(x);
    int y0 = (int)std::floor(y);
    int x1 = x0 + 1;
    int y1 = y0 + 1;
    double dx = x - x0;
    double dy = y - y0;
    auto v = [&](int yy, int xx)->double{
        if (xx < 0 || xx >= w || yy < 0 || yy >= h) return 0.0;
        return src[yy * w + xx];
    };
    double v00 = v(y0, x0);
    double v10 = v(y0, x1);
    double v01 = v(y1, x0);
    double v11 = v(y1, x1);
    double v0 = v00 * (1 - dx) + v10 * dx;
    double v1 = v01 * (1 - dx) + v11 * dx;
    double val = v0 * (1 - dy) + v1 * dy;
    if (val < 0.0) val = 0.0; if (val > 1.0) val = 1.0;
    return val;
}

void Scale(const double* src, int src_w, int src_h, double* dst, int dst_w, int dst_h, double factor) {
    if (!src || !dst) return;
    double cx = (dst_w - 1) / 2.0;
    double cy = (dst_h - 1) / 2.0;
    for (int y = 0; y < dst_h; ++y) {
        for (int x = 0; x < dst_w; ++x) {
            // map dst pixel to src coordinate with center scaling: src = center + (dst - center)/factor
            double src_x = cx + (x - cx) / factor;
            double src_y = cy + (y - cy) / factor;
            dst[y * dst_w + x] = sample_bilinear_pad(src, src_w, src_h, src_x, src_y);
        }
    }
}

void Rotate(const double* src, int src_w, int src_h, double* dst, int dst_w, int dst_h, double degrees_clockwise) {
    if (!src || !dst) return;
    double cx = (dst_w - 1) / 2.0;
    double cy = (dst_h - 1) / 2.0;
    double theta = degrees_clockwise * M_PI / 180.0; // clockwise angle
    double c = std::cos(theta);
    double s = std::sin(theta);
    for (int y = 0; y < dst_h; ++y) {
        for (int x = 0; x < dst_w; ++x) {
            double dx = x - cx;
            double dy = y - cy;
            // inverse rotation to sample source
            double src_x = c * dx + s * dy + cx;
            double src_y = -s * dx + c * dy + cy;
            dst[y * dst_w + x] = sample_bilinear_pad(src, src_w, src_h, src_x, src_y);
        }
    }
}

void ApplySaltAndPepper(const double* src, int src_w, int src_h, double* dst, int dst_w, int dst_h, double noise_fraction) {
    if (!src || !dst) return;
    // copy
    int n = dst_w * dst_h;
    for (int i = 0; i < n; ++i) dst[i] = src[i];

    if (noise_fraction <= 0.0) return;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    std::bernoulli_distribution coin(0.5);
    int total = static_cast<int>(noise_fraction * n);
    // randomly choose pixels; using probability per pixel is simpler
    for (int i = 0; i < n; ++i) {
        double r = dis(gen);
        if (r < noise_fraction) {
            dst[i] = coin(gen) ? 1.0 : 0.0;
        }
    }
}

std::vector<DataPoint*> GenerateAugmentedDataPoints(const double* src, int src_w, int src_h, int label, int num_labels) {
    std::vector<DataPoint*> out;
    if (!src) return out;

    // prepare buffers
    int w = src_w; int h = src_h;
    int n = w * h;
    double* buf_scaled = new double[n];
    double* buf_rot = new double[n];
    double* buf_noisy = new double[n];

    // scaling options: none (factor=1), scaleDown, scaleUp
    const double factors[3] = {1.0, CONSTANTS_H::SCALEDOWN, CONSTANTS_H::SCALEUP};
    // rotation options: 0, left (-angle), right (+angle) (use degrees clockwise for Rotate)
    const double rotations[3] = {0.0, -CONSTANTS_H::ROTATEANGLE, CONSTANTS_H::ROTATEANGLE};
    // noise options: false/true

    for (int si = 0; si < 3; ++si) {
        double factor = factors[si];
        // produce scaled buffer
        if (factor == 1.0) {
            for (int i = 0; i < n; ++i) buf_scaled[i] = src[i];
        } else {
            Scale(src, w, h, buf_scaled, w, h, factor);
        }

        for (int ri = 0; ri < 3; ++ri) {
            double rot = rotations[ri];
            // produce rotated buffer
            if (rot == 0.0) {
                for (int i = 0; i < n; ++i) buf_rot[i] = buf_scaled[i];
            } else {
                Rotate(buf_scaled, w, h, buf_rot, w, h, rot);
            }

            for (int noiseOn = 0; noiseOn <= 1; ++noiseOn) {
                if (noiseOn == 0) {
                    // no noise
                    for (int i = 0; i < n; ++i) buf_noisy[i] = buf_rot[i];
                } else {
                    ApplySaltAndPepper(buf_rot, w, h, buf_noisy, w, h, CONSTANTS_H::NOISE_PERCENT);
                }

                // create DataPoint from buf_noisy
                double* inputs = new double[n];
                for (int i = 0; i < n; ++i) inputs[i] = buf_noisy[i];
                DataPoint* dp = new DataPoint(inputs, n, label, num_labels);
                // DataPoint ctor copies inputs, so safe to delete
                delete[] inputs;
                out.push_back(dp);
            }
        }
    }

    delete[] buf_scaled;
    delete[] buf_rot;
    delete[] buf_noisy;
    return out;
}

}
