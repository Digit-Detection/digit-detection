#ifndef LOADLAYER_H
#define LOADLAYER_H
#include "neural/Activation_Functions/CallActivation.h"
#include <string>

struct LoadLayer {
    int num_input_nodes;
    int num_output_nodes;
    int len_weights;
    int len_biases;
    double* weights;
    double* biases;
    CallActivation* activation;

    // Save/Load to a file (stand-alone)
    static void Save(const LoadLayer& data, const std::string& filename);
    static LoadLayer* Load(const std::string& filename);

    // Save/Load to a stream (used by LoadNetwork)
    void Save(std::ostream& file) const;
    void Load(std::istream& file);
    ~LoadLayer();
};

#endif