#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H
#include "neural/Activation_Functions/ActivationType.h"

class Activations {
    public:
        // Using the function
        virtual double Activate(double* inputs, int inputs_length, int index) = 0;
        // Using the derivative of the function
        virtual double Der(double* inputs, int inputs_length, int index) = 0;
        // Gets the Activation Type of the function
        virtual ActivationType GetType() = 0;
        virtual ~Activations() = default;
};

#endif