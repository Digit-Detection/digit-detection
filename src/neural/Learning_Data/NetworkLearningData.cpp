#include "neural/Learning_Data/NetworkLearningData.h"

NetworkLearningData::NetworkLearningData(Layer** layer, int num_layers) {
    this->layer_data = new LayerLearningData*[num_layers];
    this->layer_data_size = num_layers;
    for (int i = 0; i < num_layers; i++) {
        this->layer_data[i] = new LayerLearningData(layer[i]);
    }
}

LayerLearningData* NetworkLearningData::get_layer_data(int index) {
    return this->layer_data[index];
}

NetworkLearningData::~NetworkLearningData() {
    for (int i = 0; i < this->layer_data_size; i++) {
        delete this->layer_data[i];
    }
    delete[] this->layer_data;
}