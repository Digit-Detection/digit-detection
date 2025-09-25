#include <utility>
#include <iostream>
#include "neural/Training/NetworkTrainer.h"
#include "neural/Network/NeuralNetwork.h"
#include "neural/Network/NetworkSettings.h"
#include "neural/Training/DatasetHandling.h"


NetworkTrainer::NetworkTrainer() {
    // Network Training Constructor
    this->training_split = 0.8;
    this->network_settings = new NetworkSettings();
    this->defaultConstructed = true;
    this->data_helper = DatasetHandling();
    this->evaluator = NetworkEvaluator();

    this->current_learn_rate = this->network_settings->get_initial_learning_rate();
    this->data_loaded = false;
    this->epoch = 0;
}

NetworkTrainer::NetworkTrainer(NetworkSettings* settings) {
    // Constructs Network Tester given network settings
    this->training_split = 0.8;
    this->network_settings = settings;
    this->defaultConstructed = false;
    this->data_helper = DatasetHandling();
    this->evaluator = NetworkEvaluator();

    this->current_learn_rate = this->network_settings->get_initial_learning_rate();
    this->data_loaded = false;
    this->epoch = 0;
}
        
void NetworkTrainer::StartTrainingSession(int num_epochs) {
    // Trains the network
    if (!this->data_loaded) {
        this->LoadData();
    }
    // Initializes the neural network
    NeuralNetwork* neural_network = new NeuralNetwork(this->network_settings->get_layer_sizes(), this->network_settings->get_num_layers());
    neural_network->set_activation_function(this->network_settings->get_activation_type(), this->network_settings->get_output_activation_type());
    neural_network->set_cost_function(this->network_settings->get_cost_type());
    // Learning
    for (int epoch = 1; epoch <= num_epochs; epoch++) {
        for (int batch = 0; batch < this->num_training_batches; batch++) {
            neural_network->Learn(this->training_batches[batch]->get_data(), this->network_settings->get_mini_batch_size(), this->current_learn_rate, 
                                this->network_settings->get_regularization(), this->network_settings->get_momentum());
        }
        this->epoch = epoch;
        // Evaluation
        EvaluationData* train_eval = this->evaluator.Evaluate(neural_network, this->training_data, this->training_data_length);
        EvaluationData* validation_eval = this->evaluator.Evaluate(neural_network, this->validation_data, this->validation_data_length);
        
        std::cout << "Epoch " << this->epoch << ": Training Accuracy: " << train_eval->get_num_correct() / (double)train_eval->get_total() * 100.0 << "% Validation Accuracy: " <<
        validation_eval->get_num_correct() / (double)validation_eval->get_total() * 100.0 << "%" << std::endl;

        // Next Epoch
        this->data_helper.ShuffleBatches(this->training_batches, this->num_training_batches);
        this->current_learn_rate = (1.0 / (1.0 + this->network_settings->get_learn_rate_decay() * this->epoch)) * this->network_settings->get_initial_learning_rate();

        delete train_eval;
        delete validation_eval;
    }
    delete neural_network;
}

void NetworkTrainer::LoadData() {
    // GET DATA FUNCTION
    //std::pair<DataPoint**, int> data = <INSERT DATA GETTING FUNCTION HERE>
    //this->all_data = data.first;
    //this->all_data_length = data.second;
    
    std::pair<std::pair<DataPoint**, int>, std::pair<DataPoint**, int>> result = this->data_helper.SplitData(this->all_data, this->all_data_length, this->training_split);
    this->training_data = result.first.first;
    this->training_data_length = result.first.second;
    this->validation_data = result.second.first;
    this->validation_data_length = result.second.second;

    std::pair<Batch**, int> result1 = this->data_helper.CreateMiniBatches(this->training_data, this->training_data_length, this->network_settings->get_mini_batch_size());
    this->training_batches = result1.first;
    this->num_training_batches = result1.second;
    this->data_loaded = true;
}

// Network Trainer Testing Functions (should not be used for the main project)
void NetworkTrainer::_set_data(DataPoint** data, int data_length) {
    this->all_data = data;
    this->all_data_length = data_length;
}

NetworkTrainer::~NetworkTrainer() {
    if (this->defaultConstructed) delete this->network_settings;
}