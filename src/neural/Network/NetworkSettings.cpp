#include "neural/Network/NetworkSettings.h"
#include "constants.h"

NetworkSettings::NetworkSettings() {
    /* TIPS FOR USING NETWORK HYPER_PARAMETERS
    * Initial learning rate is typically 0.001 - 0.1
    * Learn rate decay is typically 0.3 - 0.9 times less than learn rate
    * Batch size is typically a power of 2 from 2^4 - 2^8, depending on sample size
    * Momentum is typically 0.8 - 0.95
    * regularisation is typically 1e-5 - 1e-3
    * 
    * When using ReLU, learning rate needs to be lower, around 0.01 - 0.001
    * Pair softmax with crossEntropy with the best results
    * Try avoiding MeanSquaredError as it doesn't pair well with non-linear activations
    * Generally these generalisation problems: relu, softmax and crossEntropy will provide the best networks
    */
    this->activation_type = new CallActivation(CONSTANTS_H::activation_type);
    this->output_activation_type = new CallActivation(CONSTANTS_H::output_activation_type);
    this->cost_type = new CallCost(CONSTANTS_H::cost_type);
    this->initial_learning_rate = CONSTANTS_H::initial_learning_rate;
    this->learn_rate_decay = CONSTANTS_H::learn_rate_decay;
    this->mini_batch_size = CONSTANTS_H::mini_batch_size;
    this->momentum = CONSTANTS_H::momentum;
    this->regularisation = CONSTANTS_H::regularisation;
}

void NetworkSettings::setLayerSizes(int* layer_sizes) {
    this->layer_sizes = layer_sizes;
}
        
void NetworkSettings::setNumLayers(int num_layers) {
    this->num_layers = num_layers;
}
        
int* NetworkSettings::getLayerSizes() {
    return this->layer_sizes;
}
        
int NetworkSettings::getNumLayers() {
    return this->num_layers;
}
        
Activations* NetworkSettings::getActivationType() {
    return this->activation_type->get_activation();
}
        
Activations* NetworkSettings::getOutputActivationType() {
    return this->output_activation_type->get_activation();
}
        
Costs* NetworkSettings::getCostType() {
    return this->cost_type->get_activation();
}
        
double NetworkSettings::getInitialLearningRate() {
    return this->initial_learning_rate;
}
        
double NetworkSettings::getLearnRateDecay() {
    return this->learn_rate_decay;
}
        
int NetworkSettings::getMiniBatchSize() {
    return this->mini_batch_size;
}
        
double NetworkSettings::getMomentum() {
    return this->momentum;
}
        
double NetworkSettings::getRegularisation() {
    return this->regularisation;
}

NetworkSettings::~NetworkSettings() {
    delete this->activation_type;
    delete this->output_activation_type;
    delete this->cost_type;
}