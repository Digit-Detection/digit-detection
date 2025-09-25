#include "neural/Learning_Data/LayerLearningData.h"
#include "neural/Layers/Layer.h"

LayerLearningData::LayerLearningData() {
    this->weighted_inputs = nullptr;
    this->activations = nullptr;
    this->node_values = nullptr;
    this->size = 0;
}

LayerLearningData::LayerLearningData(Layer* layer) {
    this->size = layer->get_num_output_nodes();
    this->weighted_inputs = new double[this->size];
    this->activations = new double[this->size];
    this->node_values = new double[this->size];
}

void LayerLearningData::set_inputs(double* inputs) {
    this->inputs = inputs;
}

void LayerLearningData::set_weighted_inputs(int index, double value) {
    this->weighted_inputs[index] = value;
}

double* LayerLearningData::get_weighted_inputs() {
    return this->weighted_inputs;
}

int LayerLearningData::get_size() {
    return this->size;
}

void LayerLearningData::set_activations(int index, double value) {
    this->activations[index] = value;
}

double* LayerLearningData::get_activations() {
    return this->activations;
}

void LayerLearningData::set_node_values(int index, double value) {
    this->node_values[index] = value;
}
double* LayerLearningData::get_node_values() {
    return this->node_values;
}

double LayerLearningData::get_node_values(int index) {
    return this->node_values[index];
}

double LayerLearningData::get_inputs(int index) {
    return this->inputs[index];
}

LayerLearningData::~LayerLearningData() {
    //delete[] this->inputs;
    delete[] this->weighted_inputs;
    delete[] this->activations;
    delete[] this->node_values;
}