#ifndef NETWORKEVALUATOR_H
#define NETWORKEVALUATOR_H
#include "neural/Training/EvaluationData.h"
#include "neural/Network/NeuralNetwork.h"
#include "neural/Data_Handling/DataPoint.h"

class NetworkEvaluator {
    public:
        static EvaluationData* Evaluate(NeuralNetwork* network, DataPoint** data, int data_length);
};

#endif