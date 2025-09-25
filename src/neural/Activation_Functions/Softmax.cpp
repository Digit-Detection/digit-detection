#include <cmath>
#include "neural/Activation_Functions/Softmax.h"

double Softmax::Activate(double* inputs, int inputs_length, int index) {
    // Softmax Activation Function: turns node values into percentages
    double sums = 0.0;
    for (int i = 0; i < inputs_length; i++) {
        sums += std::exp(inputs[i]);
    }
    return std::exp(inputs[index]) / sums;
}

double Softmax::Der(double* inputs, int inputs_length, int index) {
    // Softmax derivative
    double sums = 0.0;
    for (int i = 0; i < inputs_length; i++) {
        sums += std::exp(inputs[i]);
    }
    double exp = std::exp(inputs[index]);

    return (exp * sums - exp * exp) / (sums * sums);
}

ActivationType Softmax::GetType() {
    return ActivationType::softmax;
}

Softmax::~Softmax() {}