#ifndef LAYER_H
#define LAYER_H
#include <utility>
#include "neural/Activation_Functions/CallActivation.h"
#include "neural/Learning_Data/LayerLearningData.h"
#include "neural/Activation_Functions/Costs.h"


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

        //Encapsulation Methods
        int get_num_input_nodes();
        int get_num_output_nodes();
        void set_activation(Activations* activation);
        
        double get_weight(int node_in, int node_out);
        int get_flat_weight_index(int input_node_index, int output_node_index);

        // Initialization with random values
        void init_random_weights();

        // Calculate Output of the layer
        double* Output(double* inputs);
        std::pair<double*, int> Output(double* inputs, int inputs_length, LayerLearningData* learn_data);

        // Apply previously calculated gradients, updating weights and biases, and resetting the gradients
        void ApplyGradients(double learn_rate, double regularization, double momentum);

        // Calculates node values for the output layer -> the partial derivative of the cost with respect to weighted input
        void OutputLayerNodeValues(LayerLearningData* learn_data, double* expected_outputs, Costs* cost);

        // Calculates node values for the hidden layers, same thing as above
        void HiddenLayerNodeValues(LayerLearningData* learn_data, Layer* old_layer, double* old_node_values, int len_old_node_values);

        // Update Gradients
        void UpdateGradients(LayerLearningData* learn_data);

        ~Layer();
};

#endif