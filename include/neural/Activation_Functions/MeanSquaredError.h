#ifndef MEANSQUAREDERROR_H
#define MEANSQUAREDERROR_H
#include "neural/Activation_Functions/Costs.h"

class MeanSquaredError: public Costs {
    public:
        double Output(double* traversed_outputs, double* expected_outputs, int outputs_length) override;
        double Der(double traversed_outputs, double expected_outputs) override;
        CostType GetType() override;
        ~MeanSquaredError() override;
};

#endif