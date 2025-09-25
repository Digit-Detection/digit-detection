#include "neural/Training/NetworkEvaluator.h"

EvaluationData* NetworkEvaluator::Evaluate(NeuralNetwork* network, DataPoint** data, int data_length) {
    // Tests the network to output its quality in terms of accuracy of the model
    EvaluationData* eval_data = new EvaluationData(data[0]->get_expected_outputs_length());
    eval_data->set_total(data_length);

    // Traverses all data from the given dataset
    for (int i = 0; i < data_length; i++) {
        // Runs through the network given one data
        double* output = network->CalculateOutputs(data[i]->get_inputs());
        int traversed_label = network->max_value_index(output);
        delete[] output;
        
        // Evaluating the correctness of the network given 1 data point
        eval_data->increment_total_per_label(data[i]->get_label());
        if (traversed_label == data[i]->get_label()) {
            // CORRECT!!!
            eval_data->increment_num_correct_per_label(data[i]->get_label());
            eval_data->increment_num_correct();
        }
        else {
            // The network got it wrong...
            eval_data->increment_falsely_labelled(data[i]->get_label());
        }
    }

    return eval_data;
}