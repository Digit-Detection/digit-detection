#include <utility>
#include <cstdlib>
#include <ctime>
#include <limits>
#include "neural/Network/NeuralNetwork.h"
#include "neural/Activation_Functions/CallCost.h"


NeuralNetwork::NeuralNetwork(int* layer_sizes, int num_layers) {
    this->layer_sizes = layer_sizes;
    this->layers_length = num_layers - 1;
    // RNG seed
    srand(static_cast<unsigned int>(time(0)));
    this->layers = new Layer*[this->layers_length];
    for (int i = 0; i < this->layers_length; i++) {
        this->layers[i] = new Layer(layer_sizes[i], layer_sizes[i + 1]);
    }

    this->output_layer_size = layer_sizes[this->layers_length];
    this->cost = new CallCost(meanSquaredError);

    this->batch_learn_data = nullptr; 
    this->batch_learn_data_length = 0;
}
        
// Neural Network Output
        
std::pair<int, double*> NeuralNetwork::Run(double* inputs) {
    double* outputs = this->CalculateOutputs(inputs);
    int neural_network_output = this->max_value_index(outputs);
    return std::make_pair(neural_network_output, outputs);
}
        
int NeuralNetwork::max_value_index(double* values) {
    double max_value = std::numeric_limits<double>::lowest();
    int index = 0;
    for (int i = 0; i < this->output_layer_size; i++) {
        if (values[i] > max_value) {
            max_value = values[i];
            index = i;
        }
    }
    return index;
}
        
double* NeuralNetwork::CalculateOutputs(double* inputs) {
    double* current_inputs = inputs;
    for (int i = 0; i < this->layers_length; i++) {
        double* next_inputs = this->layers[i]->Output(current_inputs);
        
        // Only delete if this isn't the original input array
        if (current_inputs != inputs) {
            delete[] current_inputs;
        }
        current_inputs = next_inputs;
    }
    return current_inputs;
}

// Learning
void NeuralNetwork::Learn(DataPoint** training_data, int training_data_length, double learn_rate, double regularization, double momentum) {
    // LEARN Scripts
    // Initialize batch
    if (this->batch_learn_data == NULL || this->batch_learn_data_length != training_data_length) {
        this->batch_learn_data = new NetworkLearningData*[training_data_length];
        this->batch_learn_data_length = training_data_length;
        for (int i = 0; i < this->batch_learn_data_length; i++) {
            this->batch_learn_data[i] = new NetworkLearningData(this->layers, this->layers_length);
        }
    }

    for (int i = 0; i < training_data_length; i++) {
        this->update_gradients(training_data[i], batch_learn_data[i]);
    }

    for (int i = 0; i < this->layers_length; i++) {
        layers[i]->ApplyGradients(learn_rate / training_data_length, regularization, momentum);
    }
}

void NeuralNetwork::update_gradients(DataPoint* data, NetworkLearningData* learn_data) {
    //Generate outputs from data, saving all inputs, weighted_inputs and activations for backpropagation
    double* inputs_next_layer = data->get_inputs();
    int length = data->get_inputs_length();

    for (int i = 0; i < this->layers_length; i++) {
        std::pair<double*, int> result = this->layers[i]->Output(inputs_next_layer, length, learn_data->get_layer_data(i));
        inputs_next_layer = result.first;
        length = result.second;
    }

    //Backpropagation
    int output_layer_idx = this->layers_length - 1;
    Layer* output_layer = this->layers[output_layer_idx];
    LayerLearningData* output_learn_data = learn_data->get_layer_data(output_layer_idx);

    //Update output layer gradients
    output_layer->OutputLayerNodeValues(output_learn_data, data->get_expected_outputs(), this->cost->get_activation());
    output_layer->UpdateGradients(output_learn_data);

    //Update all hidden layer gradients
    for (int i = output_layer_idx - 1; i >= 0; i--) {
        LayerLearningData* layer_learn_data = learn_data->get_layer_data(i);
        Layer* hidden_layer = this->layers[i];

        hidden_layer->HiddenLayerNodeValues(layer_learn_data, this->layers[i + 1], 
            learn_data->get_layer_data(i + 1)->get_node_values(), learn_data->get_layer_data(i + 1)->get_size());
        hidden_layer->UpdateGradients(layer_learn_data);
    }
}

// Setting functions
void NeuralNetwork::set_cost_function(Costs* cost_function) {
    delete this->cost;
    this->cost = new CallCost(cost_function->GetType());
}
        
void NeuralNetwork::set_activation_function(Activations* activation) {
    this->set_activation_function(activation, activation);
}
        
void NeuralNetwork::set_activation_function(Activations* activation, Activations* output_layer_activation) {
    for (int i = 0; i < this->layers_length - 1; i++) {
        this->layers[i]->set_activation(activation);
    }
    this->layers[this->layers_length - 1]->set_activation(output_layer_activation);
}

NeuralNetwork::~NeuralNetwork() {
    for (int i = 0; i < this->layers_length; i++) {
        delete this->layers[i];
    }
    for (int i = 0; i < this->batch_learn_data_length; i++) {
        delete this->batch_learn_data[i];
    }
    //delete[] this->layer_sizes;
    delete this->cost;
    delete[] this->layers;
    delete[] this->batch_learn_data;
}