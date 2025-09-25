#ifndef NETWORKSETTINGS_H
#define NETWORKSETTINGS_H
#include "neural/Activation_Functions/CallActivation.h"
#include "neural/Activation_Functions/CallCost.h"

class NetworkSettings {
    private:
        // ARCHITECTURE
        int* layer_sizes;
        int num_layers;

        CallActivation* activation_type;
        CallActivation* output_activation_type;
        CallCost* cost_type;

        // Learning Parameters
        double initial_learning_rate;
        double learn_rate_decay;
        int mini_batch_size;
        double momentum;
        double regularization;
    
    public:
        NetworkSettings();
        void set_layer_sizes(int* layer_sizes);
        void set_num_layers(int num_layers);
        int* get_layer_sizes();
        int get_num_layers();
        Activations* get_activation_type();
        Activations* get_output_activation_type();
        Costs* get_cost_type();
        double get_initial_learning_rate();
        double get_learn_rate_decay();
        int get_mini_batch_size();
        double get_momentum();
        double get_regularization();
        ~NetworkSettings();
};

#endif