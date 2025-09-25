#ifndef COSTS_H
#define COSTS_H
#include "neural/Activation_Functions/CostType.h"

class Costs {
    public:
        // Running the cost function
        virtual double Output(double* traversed_outputs, double* expected_outputs, int outputs_length) = 0;
        // Running the cost function derivative
        virtual double Der(double traversed_outputs, double expected_outputs) = 0;
        virtual CostType GetType() = 0;
        virtual ~Costs() = default;
};

#endif