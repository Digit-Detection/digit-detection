#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "neural/Activation_Functions/ActivationType.h"
#include "neural/Activation_Functions/CostType.h"

// CANVAS CONSTANTS
const int CANY = 50; // Canvas Y size
const int CANX = 50; // Canvas X size;
const int SCALE = 10; // Canvas Size


// NETWORK CONSTANTS
const ActivationType activation_type = ActivationType::relu;    // Hidden Layers Activation Function
const ActivationType output_activation_type = ActivationType::softmax;  // Output Layer Activation Function
const CostType cost_type = CostType::crossEntropy;  // Cost Function for Network Learning
const double initial_learning_rate = 0.01;  // Learning Rate of Network
const double learn_rate_decay = 0.009;  // Decay of Learn Rate over epochs
const int mini_batch_size = 32; // Batch size of learning data
const double momentum = 0.9;    // Momentum of velocities in gradient descent
const double regularisation = 0.0001;   // Regularisation constant for keeping velocities realistic, prevents overflow

#endif