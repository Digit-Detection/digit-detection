#include "neural/Learning_Data/LayerLearningData.h"
#include "neural/Layers/Layer.h"

LayerLearningData::LayerLearningData() {
    this->weighted_inputs = nullptr;
    this->activations = nullptr;
    this->node_values = nullptr;
    this->size = 0;
}

LayerLearningData::LayerLearningData(Layer* layer) {
    this->size = layer->getNumOutputNodes();
    this->weighted_inputs = new double[this->size];
    this->activations = new double[this->size];
    this->node_values = new double[this->size];
}

void LayerLearningData::setInputs(double* inputs) {
    this->inputs = inputs;
}

void LayerLearningData::setWeightedInputs(int index, double value) {
    this->weighted_inputs[index] = value;
}

double* LayerLearningData::getWeightedInputs() {
    return this->weighted_inputs;
}

int LayerLearningData::getSize() {
    return this->size;
}

void LayerLearningData::setActivations(int index, double value) {
    this->activations[index] = value;
}

double* LayerLearningData::getActivations() {
    return this->activations;
}

void LayerLearningData::setNodeValues(int index, double value) {
    this->node_values[index] = value;
}
double* LayerLearningData::getNodeValues() {
    return this->node_values;
}

double LayerLearningData::getNodeValues(int index) {
    return this->node_values[index];
}

double LayerLearningData::getInputs(int index) {
    return this->inputs[index];
}

LayerLearningData::~LayerLearningData() {
    //delete[] this->inputs;
    delete[] this->weighted_inputs;
    delete[] this->activations;
    delete[] this->node_values;
}