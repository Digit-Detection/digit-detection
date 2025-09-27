#ifndef NETWORKDATA_H
#define NETWORKDATA_H
#include "neural/Network/NeuralNetwork.h"
#include "neural/Data_Handling/LoadNetwork.h"
#include <string>

class NetworkData {
    private:
        double accuracy;
        std::string filename;

    public:
        explicit NetworkData(const std::string& file = "network_data.bin");

        double get_accuracy() const;
        NeuralNetwork* LoadNetworkFromSaved();
        void SaveNetworkToSaved(NeuralNetwork* net, double new_accuracy, bool compare = true);
};

#endif
