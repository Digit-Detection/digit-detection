#include <algorithm>
#include "neural/Activation_Functions/ReLU.h"

double ReLU::Activate(double* inputs, int inputs_length, int index) {
    // ReLU activation function: __/
    return std::max(0.0, inputs[index]);
}

double ReLU::Der(double* inputs, int inputs_length, int index) {
    // ReLU derivative: __--
    return (inputs[index] > 0)? 1: 0;
}

ActivationType ReLU::GetType() {
    return ActivationType::relu;
}

ReLU::~ReLU() {}