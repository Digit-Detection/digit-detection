#ifndef CROSSENTROPY_H
#define CROSSENTROPY_H
#include "neural/Activation_Functions/Costs.h"

class CrossEntropy: public Costs {
    public:
        double Output(double* traversed_outputs, double* expected_outputs, int outputs_length) override;
        double Der(double traversed_outputs, double expected_outputs) override;
        CostType GetType() override;
        ~CrossEntropy() override;
};

#endif