#include "neural/Data_Handling/LoadNetwork.h"
#include <fstream>
#include <iostream>

void LoadNetwork::Save(std::ostream& file) const {
    // Fixed size members
    file.write(reinterpret_cast<const char*>(&this->layers_length), sizeof(int));
    
    // Dynamically allocated members
    if (this->layer_sizes != nullptr && this->layers_length > 0) {
        file.write(reinterpret_cast<const char*>(this->layer_sizes), (this->layers_length + 1) * sizeof(int));
    }
    
    CostType type = this->cost ? this->cost->get_activation()->GetType() : CostType::crossEntropy;
    file.write(reinterpret_cast<const char*>(&type), sizeof(CostType));

    // Saving each layer
    for (int i = 0; i < this->layers_length; i++) {
        this->layers[i]->Save(file);
    }
}

void LoadNetwork::Load(std::istream& file) {
    //Fixed size members
    file.read(reinterpret_cast<char*>(&this->layers_length), sizeof(int));
    
    // Dynamically allocated members
    this->layer_sizes = new int[this->layers_length + 1];
    file.read(reinterpret_cast<char*>(this->layer_sizes), (this->layers_length + 1) * sizeof(int));
    
    CostType type;
    file.read(reinterpret_cast<char*>(&type), sizeof(CostType));
    this->cost = new CallCost(type);

    // Loading each layer
    this->layers = new LoadLayer*[this->layers_length];
    for (int i = 0; i < this->layers_length; i++) {
        this->layers[i] = new LoadLayer;
        this->layers[i]->Load(file);
    }
}

void LoadNetwork::Save(const LoadNetwork& data, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary | std::ios::trunc);
    if (!file) {
        throw std::runtime_error("Failed to open file for writing");
    }
    data.Save(file);
}

LoadNetwork* LoadNetwork::Load(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file for reading");
    }
    LoadNetwork* network = new LoadNetwork;
    network->Load(file);
    return network;
}

LoadNetwork::~LoadNetwork() {
    if (layers) {
        for (int i = 0; i < layers_length; i++) { 
            delete layers[i];
        }
        delete[] layers; 
    }

    delete[] layer_sizes;
    delete cost;
}