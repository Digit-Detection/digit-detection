#include <cmath>
#include "neural/Activation_Functions/CrossEntropy.h"

double CrossEntropy::Output(double* traversed_outputs, double* expected_outputs, int outputs_length) {
    // Implementation of the Cross Entropy loss function
    double cost = 0;
    for (int i = 0; i < outputs_length; i++) {
        double x = traversed_outputs[i]; 
        double y = expected_outputs[i];
        double v = (y == 1.0)? -std::log(x) : -std::log(1 - x);
        cost += std::isnan(v)? 0 : v;
    }
    return cost;
}
        
double CrossEntropy::Der(double traversed_outputs, double expected_outputs) {
    // Cross Entropy Derivative
    double x = traversed_outputs;
    double y = expected_outputs;
    if (x == 0.0 || x == 1.0) {
        return 0;
    }
    return (x - y) / (x * (1.0 - x));
}
        
CostType CrossEntropy::GetType() {
    return CostType::crossEntropy;
}

CrossEntropy::~CrossEntropy() {}