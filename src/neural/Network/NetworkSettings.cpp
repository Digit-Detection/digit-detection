#include "neural/Network/NetworkSettings.h"

NetworkSettings::NetworkSettings() {
    /* TIPS FOR USING NETWORK HYPER_PARAMETERS
    * Initial learning rate is typically 0.001 - 0.1
    * Learn rate decay is typically 0.3 - 0.9 times less than learn rate
    * Batch size is typically a power of 2 from 2^4 - 2^8, depending on sample size
    * Momentum is typically 0.8 - 0.95
    * Regularization is typically 1e-5 - 1e-3
    * 
    * When using ReLU, learning rate needs to be lower, around 0.01 - 0.001
    * Pair softmax with crossEntropy with the best results
    * Try avoiding MeanSquaredError as it doesn't pair well with non-linear activations
    * Generally these generalization problems: relu, softmax and crossEntropy will provide the best networks
    */
    this->activation_type = new CallActivation(relu);
    this->output_activation_type = new CallActivation(softmax);
    this->cost_type = new CallCost(crossEntropy);
    this->initial_learning_rate = 0.01;
    this->learn_rate_decay = 0.009;
    this->mini_batch_size = 32;
    this->momentum = 0.9;
    this->regularization = 0.0001;
}

void NetworkSettings::set_layer_sizes(int* layer_sizes) {
    this->layer_sizes = layer_sizes;
}
        
void NetworkSettings::set_num_layers(int num_layers) {
    this->num_layers = num_layers;
}
        
int* NetworkSettings::get_layer_sizes() {
    return this->layer_sizes;
}
        
int NetworkSettings::get_num_layers() {
    return this->num_layers;
}
        
Activations* NetworkSettings::get_activation_type() {
    return this->activation_type->get_activation();
}
        
Activations* NetworkSettings::get_output_activation_type() {
    return this->output_activation_type->get_activation();
}
        
Costs* NetworkSettings::get_cost_type() {
    return this->cost_type->get_activation();
}
        
double NetworkSettings::get_initial_learning_rate() {
    return this->initial_learning_rate;
}
        
double NetworkSettings::get_learn_rate_decay() {
    return this->learn_rate_decay;
}
        
int NetworkSettings::get_mini_batch_size() {
    return this->mini_batch_size;
}
        
double NetworkSettings::get_momentum() {
    return this->momentum;
}
        
double NetworkSettings::get_regularization() {
    return this->regularization;
}

NetworkSettings::~NetworkSettings() {
    delete this->activation_type;
    delete this->output_activation_type;
    delete this->cost_type;
}