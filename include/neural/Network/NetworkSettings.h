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
        double regularisation;
    
    public:
        NetworkSettings();
        void setLayerSizes(int* layer_sizes);
        void setNumLayers(int num_layers);
        int* getLayerSizes();
        int getNumLayers();
        Activations* getActivationType();
        Activations* getOutputActivationType();
        Costs* getCostType();
        double getInitialLearningRate();
        double getLearnRateDecay();
        int getMiniBatchSize();
        double getMomentum();
        double getRegularisation();
        ~NetworkSettings();
};

#endif