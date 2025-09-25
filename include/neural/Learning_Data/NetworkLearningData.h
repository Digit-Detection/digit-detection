#ifndef NETWORKLEARINGDATA_H
#define NETWORKLEARNINGDATA_H
#include "neural/Learning_Data/LayerLearningData.h"

class NetworkLearningData {
    private:
        LayerLearningData** layer_data;
        int layer_data_size;
    public:
        NetworkLearningData(Layer** layer, int num_layers);
        LayerLearningData* get_layer_data(int index);
        ~NetworkLearningData();
};

#endif