#ifndef LAYER_H
#define LAYER_H
#include <utility>
#include "neural/Activation_Functions/CallActivation.h"
#include "neural/Learning_Data/LayerLearningData.h"
#include "neural/Activation_Functions/Costs.h"
#include "neural/Data_Handling/LoadLayer.h"


class Layer {
    private:
        int num_input_nodes;
        int num_output_nodes;

        int len_weights;
        int len_biases;

        double* weights;
        double* biases;

        double* cost_gradient_w;
        double* cost_gradient_b;

        double* weight_vels;
        double* bias_vels;

        CallActivation* activation;
    public:
        Layer(int num_input_nodes, int num_output_nodes);
        Layer(LoadLayer* layer_params);

        //Encapsulation Methods
        LoadLayer* getLayerData();
        int getNumInputNodes();
        int getNumOutputNodes();
        void setActivation(Activations* activation);
        
        double getWeight(int node_in, int node_out);
        int getFlatWeightIndex(int input_node_index, int output_node_index);

        // Initialization with random values
        void initRandomWeights();

        // Calculate Output of the layer
        double* Output(double* inputs);
        std::pair<double*, int> Output(double* inputs, int inputs_length, LayerLearningData* learn_data);

        // Apply previously calculated gradients, updating weights and biases, and resetting the gradients
        void ApplyGradients(double learn_rate, double regularisation, double momentum);

        // Calculates node values for the output layer -> the partial derivative of the cost with respect to weighted input
        void OutputLayerNodeValues(LayerLearningData* learn_data, double* expected_outputs, Costs* cost);

        // Calculates node values for the hidden layers, same thing as above
        void HiddenLayerNodeValues(LayerLearningData* learn_data, Layer* old_layer, double* old_node_values, int len_old_node_values);

        // Update Gradients
        void UpdateGradients(LayerLearningData* learn_data);

        ~Layer();
};

#endif