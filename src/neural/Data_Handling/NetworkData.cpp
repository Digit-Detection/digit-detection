#include "neural/Data_Handling/NetworkData.h"
#include <fstream>
#include <iostream>

NetworkData::NetworkData(const std::string& file) {
    this->accuracy = 0.0;
    this->filename = file;
}


double NetworkData::get_accuracy() const {
    return this->accuracy;
}


NeuralNetwork* NetworkData::LoadNetworkFromSaved() {
    //Loads network from saved data
    std::ifstream file(filename, std::ios::binary);
    // If the file is missing or cannot be opened, return nullptr and callers should handle missing network.
    if (!file.is_open()) {
        std::cerr << "Could not open " << filename << std::endl;
        return nullptr;
    }

    // Accuracy reading
    file.read(reinterpret_cast<char*>(&this->accuracy), sizeof(double));

    // Network Loading
    LoadNetwork* loader = new LoadNetwork;
    loader->Load(file);

    // Reconstruct a usable NeuralNetwork from the LoadNetwork
    NeuralNetwork* network = new NeuralNetwork(loader);

    delete loader;

    return network;
}

// Save accuracy + network if allowed by compare
void NetworkData::SaveNetworkToSaved(NeuralNetwork* network, double new_accuracy, bool compare) {
    if (compare && new_accuracy <= this->accuracy) {
        return; // do not overwrite if accuracy is worse
    }

    std::ofstream file(filename, std::ios::binary | std::ios::trunc);
    // Open the network data file for writing (truncate). This will create the file if it does not exist.
    if (!file) {
        std::cerr << "Could not open " << filename << " for writing" << std::endl;
        return;
    }

    // Convert live network to a LoadNetwork object
    LoadNetwork* saver = network->get_network_data();

    // Accuracy writing
    file.write(reinterpret_cast<const char*>(&new_accuracy), sizeof(double));

    // Network writing
    saver->Save(file);

    this->accuracy = new_accuracy;

    // Confirmation Message
    std::cout << "Saved the current network!" << std::endl;

    // Clean up
    delete saver;
}
