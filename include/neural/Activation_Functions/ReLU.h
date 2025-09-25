#ifndef RELU_H
#define RELUH
#include "neural/Activation_Functions/Activations.h"

class ReLU : public Activations {
    public:
        double Activate(double* inputs, int inputs_length, int index) override;
        double Der(double* inputs, int inputs_length, int index) override;
        ActivationType GetType() override;
        ~ReLU() override;
};

#endif