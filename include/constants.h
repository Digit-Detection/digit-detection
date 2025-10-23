#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "neural/Activation_Functions/ActivationType.h"
#include "neural/Activation_Functions/CostType.h"

// GUI CONSTANTS
const int CANY = 500; // Canvas Y sizes
const int CANX = 500; // Canvas X size;
const int CANSCALE = 1; // Canvas Shown Scaled size
const int TOOLSIZE = 300; // In pixels

// NETWORK CONSTANTS
const ActivationType activation_type = ActivationType::relu;    // Hidden Layers Activation Function
const ActivationType output_activation_type = ActivationType::softmax;  // Output Layer Activation Function
const CostType cost_type = CostType::crossEntropy;  // Cost Function for Network Learning
const double initial_learning_rate = 0.01;  // Learning Rate of Network
const double learn_rate_decay = 0.009;  // Decay of Learn Rate over epochs
const int mini_batch_size = 32; // Batch size of learning data
const double momentum = 0.9;    // Momentum of velocities in gradient descent
const double regularisation = 0.0001;   // Regularisation constant for keeping velocities realistic, prevents overflow

// NETWORK ARCHETECTURE
const int input_layer_size = 28 * 28; // Input layer of neural network size
const int output_layer_size = 10; // Output layer of neural network size

// Destination (resample) size for saved drawings. Change these to alter
// the size drawings are scaled to before being stored / used for training.
const int DESTX = 28; // Destination X size in pixels (MNIST is 28)
const int DESTY = 28; // Destination Y size in pixels (MNIST is 28)


// AUGMENTATION CONSTANTS (arrays of values)
static const double SCALE_FACTORS[] = {0.85, 0.9, 0.95, 1.05, 1.1, 1.15};
static const int NUM_SCALE_FACTORS = 6;

static const double ROTATE_ANGLES[] = {5.0, 10.0, 15.0};
static const int NUM_ROTATE_ANGLES = 3;

static const double NOISE_LEVELS[] = {0.005, 0.006, 0.007, 0.008, 0.009, 0.01};
static const int NUM_NOISE_LEVELS = 6;

#endif