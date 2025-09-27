#include "neural/Data_Handling/LoadLayer.h"
#include <fstream>
#include <iostream>

// Stream Based
void LoadLayer::Save(std::ostream& file) const {
    //Fixed size members
    file.write(reinterpret_cast<const char*>(&this->num_input_nodes), sizeof(int));
    file.write(reinterpret_cast<const char*>(&this->num_output_nodes), sizeof(int));
    file.write(reinterpret_cast<const char*>(&this->len_weights), sizeof(int));
    file.write(reinterpret_cast<const char*>(&this->len_biases), sizeof(int));
    
    // Dynamically allocated members
    if (this->weights != nullptr && this->len_weights > 0) {
        file.write(reinterpret_cast<const char*>(this->weights), sizeof(double) * this->len_weights);
    }
    if (this->biases != nullptr && this->len_biases > 0) {
        file.write(reinterpret_cast<const char*>(this->biases),  sizeof(double) * this->len_biases);
    }
    
    ActivationType type = this->activation ? this->activation->get_activation()->GetType() : ActivationType::sigmoid;
    file.write(reinterpret_cast<const char*>(&type), sizeof(ActivationType));

}

void LoadLayer::Load(std::istream& file) {
    this->weights = nullptr;
    this->biases = nullptr;
    this->activation = nullptr;

    //Fixed size members
    file.read(reinterpret_cast<char*>(&this->num_input_nodes), sizeof(int));
    file.read(reinterpret_cast<char*>(&this->num_output_nodes), sizeof(int));
    file.read(reinterpret_cast<char*>(&this->len_weights), sizeof(int));
    file.read(reinterpret_cast<char*>(&this->len_biases), sizeof(int));
    
    // Dynamically allocated members
    if (this->len_weights > 0) {
        this->weights = new double[this->len_weights];
        file.read(reinterpret_cast<char*>(this->weights), this->len_weights * sizeof(double));
    }
    if (this->len_biases > 0) {
        this->biases = new double[this->len_biases];
        file.read(reinterpret_cast<char*>(this->biases), this->len_biases * sizeof(double));
    }
    ActivationType type;
    file.read(reinterpret_cast<char*>(&type), sizeof(ActivationType));
    this->activation = new CallActivation(type);
}


// File Based
void LoadLayer::Save(const LoadLayer& data, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary | std::ios::out);
    if (!file) throw std::runtime_error("Failed to open file for writing");
    data.Save(file);
}


LoadLayer* LoadLayer::Load(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::in);
    if (!file) throw std::runtime_error("Failed to open file for reading");
    LoadLayer* layer = new LoadLayer;
    layer->Load(file);
    return layer;
}

LoadLayer::~LoadLayer() {
    if (this->weights != nullptr) {
        delete[] this->weights;
    }
    if (this->biases != nullptr) {
        delete[] this->biases;
    }
    if (this->activation != nullptr) {
        delete this->activation;
    }
}