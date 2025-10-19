#include <utility>
#include <iostream>
#include "neural/Training/NetworkTrainer.h"
#include "neural/Network/NeuralNetwork.h"
#include "neural/Network/NetworkSettings.h"
#include "neural/Training/DatasetHandling.h"
#include "neural/Data_Handling/NetworkData.h"
#include "neural/Data_Handling/MnistLoader.h"
#include "neural/Data_Handling/DataSet.h"
#include <fstream>


NetworkTrainer::NetworkTrainer() {
    // Network Training Constructor
    this->training_split = 0.8;
    this->network_settings = new NetworkSettings();
    this->defaultConstructed = true;

    this->current_learn_rate = this->network_settings->get_initial_learning_rate();
    this->data_loaded = false;
    this->epoch = 0;
}

NetworkTrainer::NetworkTrainer(NetworkSettings* settings) {
    // Constructs Network Tester given network settings
    this->training_split = 0.8;
    this->network_settings = settings;
    this->defaultConstructed = false;

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
                                this->network_settings->get_regularisation(), this->network_settings->get_momentum());
        }
        this->epoch = epoch;
        // Evaluation
        EvaluationData* train_eval = NetworkEvaluator::Evaluate(neural_network, this->training_data, this->training_data_length);
        EvaluationData* validation_eval = NetworkEvaluator::Evaluate(neural_network, this->validation_data, this->validation_data_length);
        
        std::cout << "Epoch " << this->epoch << ": Training Accuracy: " << train_eval->get_num_correct() / (double)train_eval->get_total() * 100.0 << "% Validation Accuracy: " <<
        validation_eval->get_num_correct() / (double)validation_eval->get_total() * 100.0 << "%" << std::endl;

        // Next Epoch
        DatasetHandling::ShuffleBatches(this->training_batches, this->num_training_batches);
        this->current_learn_rate = (1.0 / (1.0 + this->network_settings->get_learn_rate_decay() * this->epoch)) * this->network_settings->get_initial_learning_rate();

        delete train_eval;
        delete validation_eval;
    }
    // New network handling
    NetworkData network_data = NetworkData();
    NeuralNetwork* prev_network = network_data.LoadNetworkFromSaved();
    EvaluationData* validation_eval = NetworkEvaluator::Evaluate(neural_network, this->validation_data, this->validation_data_length);
    network_data.SaveNetworkToSaved(neural_network, validation_eval->get_num_correct() / (double)validation_eval->get_total());
    
    delete prev_network;
    delete validation_eval;
    delete neural_network;
}

void NetworkTrainer::LoadData() {
    // If no data was previously injected, try loading mnist.bin (convert from IDX if needed)
    if (this->all_data == nullptr || this->all_data_length == 0) {
        const std::string mnist_bin = "data/mnist.bin";
        // If mnist.bin doesn't exist, attempt to convert from IDX files in data/
        std::ifstream f(mnist_bin, std::ios::binary);
        if (!f) {
            const std::string img = "data/train-images.idx3-ubyte";
            const std::string lab = "data/train-labels.idx1-ubyte";
            std::cout << "mnist.bin not found, converting MNIST IDX files to " << mnist_bin << "..." << std::endl;
            bool ok = MnistLoader::ConvertToBin(img, lab, mnist_bin, 10);
            if (!ok) std::cerr << "Warning: failed to convert MNIST to bin; proceeding without MNIST." << std::endl;
        } else {
            f.close();
        }

        // Load mnist.bin if available
        DataPoint** mnist_arr = nullptr;
        int mnist_n = 0;
        std::pair<DataPoint**, int> loaded_mnist = DataSet::LoadDataPoints(mnist_bin);
        if (loaded_mnist.second > 0 && loaded_mnist.first != nullptr) {
            mnist_arr = loaded_mnist.first;
            mnist_n = loaded_mnist.second;
            std::cout << "Loaded " << mnist_n << " samples from " << mnist_bin << "\n";
        }

        // Load user drawings: prefer data/user_drawings.bin, fall back to user_drawings.bin
        DataPoint** user_arr = nullptr;
        int user_n = 0;
        const std::string user_path1 = "data/user_drawings.bin";
        const std::string user_path2 = "user_drawings.bin";
        std::ifstream fu(user_path1, std::ios::binary);
        std::string user_path_to_use;
        if (fu) { user_path_to_use = user_path1; fu.close(); }
        else {
            std::ifstream fu2(user_path2, std::ios::binary);
            if (fu2) { user_path_to_use = user_path2; fu2.close(); }
        }
        if (!user_path_to_use.empty()) {
            std::pair<DataPoint**, int> loaded_user = DataSet::LoadDataPoints(user_path_to_use);
            if (loaded_user.second > 0 && loaded_user.first != nullptr) {
                user_arr = loaded_user.first;
                user_n = loaded_user.second;
                std::cout << "Loaded " << user_n << " samples from " << user_path_to_use << "\n";
            }
        }

        // Combine arrays if necessary (preserve originals)
        if ((mnist_n + user_n) > 0) {
            int total = mnist_n + user_n;
            DataPoint** combined = new DataPoint*[total];
            int idx = 0;
            if (mnist_arr) {
                for (int i = 0; i < mnist_n; ++i) combined[idx++] = mnist_arr[i];
                // free the mnist pointer array (not the DataPoints)
                delete [] mnist_arr;
            }
            if (user_arr) {
                for (int i = 0; i < user_n; ++i) combined[idx++] = user_arr[i];
                delete [] user_arr;
            }
            this->all_data = combined;
            this->all_data_length = total;
            std::cout << "Combined dataset size: " << this->all_data_length << " datapoints\n";
        }
    }

    // If still no data, fall back to existing behavior (this->all_data must have been set by tests)
    std::pair<std::pair<DataPoint**, int>, std::pair<DataPoint**, int>> result = DatasetHandling::SplitData(this->all_data, this->all_data_length, this->training_split);
    this->training_data = result.first.first;
    this->training_data_length = result.first.second;
    this->validation_data = result.second.first;
    this->validation_data_length = result.second.second;

    std::pair<Batch**, int> result1 = DatasetHandling::CreateMiniBatches(this->training_data, this->training_data_length, this->network_settings->get_mini_batch_size());
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