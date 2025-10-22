#include <cstdlib>
#include <cmath>
#include <utility>
#include "neural/Layers/Layer.h"
#include "neural/Activation_Functions/CallActivation.h"


Layer::Layer(int num_input_nodes, int num_output_nodes) {
    // Construction
    this->num_input_nodes = num_input_nodes;
    this->num_output_nodes = num_output_nodes;
    this->activation = new CallActivation(sigmoid);

    this->len_weights = num_input_nodes * num_output_nodes;
    this->len_biases = num_output_nodes;
    this->weights = new double[this->len_weights];
    this->cost_gradient_w = new double[this->len_weights];
    this->biases = new double[this->len_biases];
    this->cost_gradient_b = new double[this->len_biases];

    this->weight_vels = new double[this->len_weights];
    this->bias_vels = new double[this->len_biases];

    this->initRandomWeights();
    // Initializing biases
    for (int i = 0; i < this->len_biases; i++) {
        this->biases[i] = 0.0;
        this->cost_gradient_b[i] = 0.0;
        this->bias_vels[i] = 0.0;
    }
}

Layer::Layer(LoadLayer* layer_params) {
    // Construction using shallow copying of LoadLayer, which arrays are already allocated on the heap, shallow copying
    this->num_input_nodes = layer_params->num_input_nodes;
    this->num_output_nodes = layer_params->num_output_nodes;
    this->activation = new CallActivation(layer_params->activation->get_activation()->GetType());

    this->len_weights = layer_params->len_weights;
    this->len_biases = layer_params->len_biases;

    this->weights = new double[this->len_weights];
    for (int i = 0; i < this->len_weights; i++) {
        this->weights[i] = layer_params->weights[i];
    }

    this->biases = new double[this->len_biases];
    for (int i = 0; i < this->len_biases; i++) {
        this->biases[i] = layer_params->biases[i];
    }

    this->cost_gradient_w = new double[this->len_weights];
    this->cost_gradient_b = new double[this->len_biases];
    this->weight_vels = new double[this->len_weights];
    this->bias_vels = new double[this->len_biases];

    for (int i = 0; i < this->len_weights; i++) {
        this->cost_gradient_w[i] = 0.0;
        this->weight_vels[i] = 0.0;
    }

    for (int i = 0; i < this->len_biases; i++) {
        this->cost_gradient_b[i] = 0.0;
        this->bias_vels[i] = 0.0;
    }
}

LoadLayer* Layer::getLayerData() {
    // Returns a loadlayer struct for saving the layer data
    LoadLayer* layer_params = new LoadLayer();
    layer_params->num_input_nodes = this->num_input_nodes;
    layer_params->num_output_nodes = this->num_output_nodes;
    layer_params->len_weights = this->len_weights;
    layer_params->len_biases = this->len_biases;

    layer_params->weights = new double[this->len_weights];
    for (int i = 0; i < this->len_weights; i++) {
        layer_params->weights[i] = this->weights[i];
    }
    layer_params->biases = new double[this->len_biases];
    for (int i = 0; i < this->len_biases; i++) {
        layer_params->biases[i] = this->biases[i];
    }
    layer_params->activation = new CallActivation(this->activation->get_activation()->GetType());
    
    return layer_params;
}

//Encapsulation Methods
int Layer::getNumInputNodes() {
    return this->num_input_nodes;
}
        
int Layer::getNumOutputNodes() {
    return this->num_output_nodes;
}
        
void Layer::setActivation(Activations* activation) {
    delete this->activation;
    this->activation = new CallActivation(activation->GetType());
}
        
double Layer::getWeight(int input_node, int output_node) {
    int flat_index = output_node * this->num_input_nodes + input_node;
    return this->weights[flat_index];
}

int Layer::getFlatWeightIndex(int input_node_index, int output_node_index) {
    return output_node_index * this->num_input_nodes + input_node_index;
}

// Initialization with random values
void Layer::initRandomWeights() {
    // Xavier/He initialization
    double scale;
    if (this->activation->get_activation()->GetType() == relu) {
        // He initialization for ReLU
        scale = std::sqrt(2.0 / num_input_nodes);
    }
    else {
        // Xavier initialization
        scale = std::sqrt(1.0 / num_input_nodes);
    }
    
    for (int i = 0; i < this->len_weights; i++) {
        // Xavier/He
        this->weights[i] = ((static_cast<double>(rand()) / static_cast<double>(RAND_MAX)) * 2.0 - 1.0) * scale;
        this->cost_gradient_w[i] = 0.0;
        this->weight_vels[i] = 0.0;
    }
}

// Calculate Output of the layer
double* Layer::Output(double* inputs) {
    // Weighted inputs stores all node values before activation: winput_i = b_i + input_i * w_i
    double* weighted_inputs = new double[this->num_output_nodes];

    for (int output_node = 0; output_node < this->num_output_nodes; output_node++) {
        double weighted_input = this->biases[output_node];

        for (int input_node = 0; input_node < this->num_input_nodes; input_node++) {
            weighted_input += inputs[input_node] * this->getWeight(input_node, output_node);
        }
        weighted_inputs[output_node] = weighted_input;
    }

    // Activate all weighted inputs using the activation function
    double* activated = new double[this->num_output_nodes];
    for (int output_node = 0; output_node < this->num_output_nodes; output_node++) {
        activated[output_node] = this->activation->get_activation()->Activate(weighted_inputs, this->num_output_nodes, output_node);
    }
    delete[] weighted_inputs;

    return activated;
}
        
std::pair<double*, int> Layer::Output(double* inputs, int inputs_length, LayerLearningData* learn_data) {
    // Same functions as above but for LayerLearningData: this is for learning
    learn_data->setInputs(inputs);

    for (int output_node = 0; output_node < this->num_output_nodes; output_node++) {
        double weighted_input = this->biases[output_node];

        for (int input_node = 0; input_node < this->num_input_nodes; input_node++) {
            weighted_input += inputs[input_node] * this->getWeight(input_node, output_node);
        }
        learn_data->setWeightedInputs(output_node, weighted_input);
    }

    for (int output_node = 0; output_node < learn_data->getSize(); output_node++) {
        learn_data->setActivations(output_node, this->activation->get_activation()->Activate(learn_data->getWeightedInputs(), this->num_output_nodes, output_node));
    }

    return std::make_pair(learn_data->getActivations(), learn_data->getSize());
}

// Apply previously calculated gradients, updating weights and biases, and resetting the gradients
void Layer::ApplyGradients(double learn_rate, double regularisation, double momentum) {
    // Weight decay is for weight clipping in case those values are too extreme
    double weight_decay = 1.0 - regularisation * learn_rate;

    // Applying to weight
    // w = (1 − ηλ)w + μv_(t−1) ​− η∇w (η: learn rate, λ: regulatization, μ: momentum, w: weight, ∇w: cost in change in weight)
    for (int i = 0; i < this->len_weights; i++) {
        double weight = this->weights[i];
        // Velocity of gradient calculation -> essentially how big in magnitude the gradient is
        double velocity = this->weight_vels[i] * momentum - this->cost_gradient_w[i] * learn_rate;
        this->weight_vels[i] = velocity;
        this->weights[i] = weight * weight_decay + velocity;
        this->cost_gradient_w[i] = 0;
    }

    // Applying the same for biases
    for (int i = 0; i < this->len_biases; i++) {
        double velocity = this->bias_vels[i] * momentum - this->cost_gradient_b[i] * learn_rate;
        this->bias_vels[i] = velocity;
        this->biases[i] += velocity;
        this->cost_gradient_b[i] = 0;
    }
}

// Calculates node values for the output layer -> the partial derivative of the cost with respect to weighted input
void Layer::OutputLayerNodeValues(LayerLearningData* learn_data, double* expected_outputs, Costs* cost) {
    for (int i = 0; i < learn_data->getSize(); i++) {
        // Partial Derivatives for the current node: cost/activation and activation/weighted_input
        double cost_derivative = cost->Der(learn_data->getActivations()[i], expected_outputs[i]);
        double activation_derivative = this->activation->get_activation()->Der(learn_data->getWeightedInputs(), learn_data->getSize(), i);
        learn_data->setNodeValues(i, cost_derivative * activation_derivative);
    }

}

// Calculates node values for the hidden layers, same thing as above
void Layer::HiddenLayerNodeValues(LayerLearningData* learn_data, Layer* old_layer, double* old_node_values, int len_old_node_values) {
    for (int new_node_index = 0; new_node_index < this->num_output_nodes; new_node_index++) {
        double new_node_value = 0.0;
        for (int old_node_index = 0; old_node_index < len_old_node_values; old_node_index++) {
            // Partial Derivative of the weighted input with respect to the input
            double weighted_input_derivative = old_layer->getWeight(new_node_index, old_node_index);
            new_node_value += weighted_input_derivative * old_node_values[old_node_index];
        }
        new_node_value *= this->activation->get_activation()->Der(learn_data->getWeightedInputs(), learn_data->getSize(), new_node_index);
        learn_data->setNodeValues(new_node_index, new_node_value);
    }
}

// Update Gradients
void Layer::UpdateGradients(LayerLearningData* learn_data) {
    for (int output_node = 0; output_node < this->num_output_nodes; output_node++) {
        double node_value = learn_data->getNodeValues(output_node);
        for (int input_node = 0; input_node < this->num_input_nodes; input_node++) {
            // Partial Derivative of cost/weight of current connection
            double derivativeCost_Weight = learn_data->getInputs(input_node) * node_value;
            // cost_gradient_w stores these partial derivatives for each weight
            // NOTE: Gradient is added instead of set to calculate average gradient for the batch
            this->cost_gradient_w[this->getFlatWeightIndex(input_node, output_node)] += derivativeCost_Weight;
        }
    }

    for (int output_node = 0; output_node < this->num_output_nodes; output_node++) {
        // Partial Derivative of cost/bias
        double derivativeCost_Bias = 1.0 * learn_data->getNodeValues(output_node);
        this->cost_gradient_b[output_node] += derivativeCost_Bias;
    }
}

Layer::~Layer() {
    delete[] this->weights;
    delete[] this->weight_vels;
    delete[] this->cost_gradient_w;
    delete[] this->biases;
    delete[] this->bias_vels;
    delete[] this->cost_gradient_b;
    delete this->activation;
}