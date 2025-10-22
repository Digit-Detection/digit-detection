#include "neural/Training/NetworkEvaluator.h"

EvaluationData* NetworkEvaluator::Evaluate(NeuralNetwork* network, DataPoint** data, int data_length) {
    // Tests the network to output its quality in terms of accuracy of the model
    EvaluationData* eval_data = new EvaluationData(data[0]->getExpectedOutputsLength());
    eval_data->setTotal(data_length);

    // Traverses all data from the given dataset
    for (int i = 0; i < data_length; i++) {
        // Runs through the network given one data
        double* output = network->CalculateOutputs(data[i]->getInputs());
        int traversed_label = network->maxValueIndex(output);
        delete[] output;
        
        // Evaluating the correctness of the network given 1 data point
        eval_data->incrementTotalPerLabel(data[i]->getLabel());
        if (traversed_label == data[i]->getLabel()) {
            // CORRECT!!!
            eval_data->incrementNumCorrectPerLabel(data[i]->getLabel());
            eval_data->incrementNumCorrect();
        }
        else {
            // The network got it wrong...
            eval_data->incrementFalselyLabelled(data[i]->getLabel());
        }
    }

    return eval_data;
}