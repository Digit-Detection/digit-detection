#ifndef LOADNETWORK_H
#define LOADNETWORK_H
#include "neural/Data_Handling/LoadLayer.h"
#include "neural/Activation_Functions/CallCost.h"

struct LoadNetwork {
    int layers_length;
    int* layer_sizes;
    CallCost* cost;
    
    LoadLayer** layers;

    // Save / Load to a file
    void Save(const LoadNetwork& data, const std::string& filename);
    LoadNetwork* Load(const std::string& filename);

    // Save / Load to an existing stream
    void Save(std::ostream& file) const;
    void Load(std::istream& file);
    ~LoadNetwork();
};

#endif