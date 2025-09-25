#ifndef SOFTMAX_H
#define SOFTMAX_H
#include "neural/Activation_Functions/Activations.h"

class Softmax : public Activations {
    public:
        double Activate(double* inputs, int inputs_length, int index) override;
        double Der(double* inputs, int inputs_length, int index) override;
        ActivationType GetType() override;
        ~Softmax() override;
};  

#endif