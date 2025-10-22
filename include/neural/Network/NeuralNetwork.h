#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H
#include <utility>
#include "neural/Layers/Layer.h"
#include "neural/Activation_Functions/CallCost.h"
#include "neural/Learning_Data/NetworkLearningData.h"
#include "neural/Data_Handling/DataPoint.h"
#include "neural/Data_Handling/LoadNetwork.h"

class NeuralNetwork {
    private:
        Layer** layers;
        int layers_length;
        int* layer_sizes;
        CallCost* cost;
        NetworkLearningData** batch_learn_data;
        int batch_learn_data_length;

        int output_layer_size;
    public:
        NeuralNetwork(int* layer_sizes, int num_layers);
        NeuralNetwork(LoadNetwork* network_params);

        // Saving network function
        LoadNetwork* getNetworkData();

        // Neural Network Output
        std::pair<int, double*> Run(double* inputs);
        int maxValueIndex(double* values);
        double* CalculateOutputs(double* inputs);

        // Learning
        void Learn(DataPoint** training_data, int training_data_length, double learn_rate, double regularisation = 0.0, double momentum = 0.0);
        void updateGradients(DataPoint* data, NetworkLearningData* learn_data);

        // Setting functions
        void set_cost_function(Costs* cost_function);
        void setActivationFunction(Activations* activation);
        void setActivationFunction(Activations* activation, Activations* output_layer_activation);

        ~NeuralNetwork();
};

#endif