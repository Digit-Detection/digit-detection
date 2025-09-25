#include <cmath>
#include "neural/Activation_Functions/Sigmoid.h"

double Sigmoid::Activate(double* inputs, int inputs_length, int index) {
    // Sigmoid Function: __/--
    return 1.0 / (1 + std::exp(-inputs[index]));
}

double Sigmoid::Der(double* inputs, int inputs_length, int index) {
    // Sigmoid derivative
    double a = Activate(inputs, inputs_length, index);
    return a * (1 - a);
}

ActivationType Sigmoid::GetType() {
    return ActivationType::sigmoid;
}

Sigmoid::~Sigmoid() {}