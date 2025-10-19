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
#include <vector>
#include <string>


NetworkTrainer::NetworkTrainer() {
    // Network Training Constructor
    this->training_split = 0.8;
    this->network_settings = new NetworkSettings();
    this->defaultConstructed = true;

    this->all_data = nullptr;
    this->all_data_length = 0;
    this->training_data = nullptr;
    this->training_data_length = 0;
    this->validation_data = nullptr;
    this->validation_data_length = 0;
    this->training_batches = nullptr;
    this->num_training_batches = 0;
    
    this->current_learn_rate = this->network_settings->get_initial_learning_rate();
    this->data_loaded = false;
    this->epoch = 0;
}

NetworkTrainer::NetworkTrainer(NetworkSettings* settings) {
    // Constructs Network Tester given network settings
    this->training_split = 0.8;
    this->network_settings = settings;
    this->defaultConstructed = false;

    this->all_data = nullptr;
    this->all_data_length = 0;
    this->training_data = nullptr;
    this->training_data_length = 0;
    this->validation_data = nullptr;
    this->validation_data_length = 0;
    this->training_batches = nullptr;
    this->num_training_batches = 0;

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
        // Try multiple candidate paths to support running from build/ or repo root
        auto find_first = [](const std::vector<std::string>& cands) -> std::string {
            for (const auto &p : cands) {
                std::ifstream f(p, std::ios::binary);
                if (f) { f.close(); return p; }
            }
            return std::string();
        };

        const std::vector<std::string> mnist_bin_cands = {"data/mnist.bin", "../data/mnist.bin", "../../data/mnist.bin"};
        std::string mnist_bin_found = find_first(mnist_bin_cands);
        if (mnist_bin_found.empty()) {
            // Try to convert from IDX files in likely locations
            const std::vector<std::string> img_cands = {"data/train-images.idx3-ubyte", "../data/train-images.idx3-ubyte", "../../data/train-images.idx3-ubyte"};
            const std::vector<std::string> lab_cands = {"data/train-labels.idx1-ubyte", "../data/train-labels.idx1-ubyte", "../../data/train-labels.idx1-ubyte"};
            std::string img_found = find_first(img_cands);
            std::string lab_found = find_first(lab_cands);
            // Compute a sensible target path next to the found IDX files so we can write there
            std::string target_bin;
            if (!img_found.empty()) {
                auto pos = img_found.find_last_of("/\\");
                if (pos != std::string::npos) target_bin = img_found.substr(0, pos) + "/mnist.bin";
                else target_bin = std::string("mnist.bin");
            } else {
                target_bin = mnist_bin_cands[0];
            }
            std::cout << "mnist.bin not found, attempting to convert MNIST IDX files to " << target_bin << "..." << std::endl;
            if (!img_found.empty() && !lab_found.empty()) {
                bool ok = MnistLoader::ConvertToBin(img_found, lab_found, target_bin, 10);
                if (!ok) std::cerr << "Warning: failed to convert MNIST to bin; proceeding without MNIST." << std::endl;
                else mnist_bin_found = target_bin;
            } else {
                std::cerr << "Failed to find MNIST IDX files in expected locations." << std::endl;
            }
        }

        // Load mnist.bin if available
        DataPoint** mnist_arr = nullptr;
        int mnist_n = 0;
        if (!mnist_bin_found.empty()) {
            std::pair<DataPoint**, int> loaded_mnist = DataSet::LoadDataPoints(mnist_bin_found);
            if (loaded_mnist.second > 0 && loaded_mnist.first != nullptr) {
                mnist_arr = loaded_mnist.first;
                mnist_n = loaded_mnist.second;
                std::cout << "Loaded " << mnist_n << " samples from " << mnist_bin_found << "\n";
            } else {
                std::cerr << "Could not open dataset file: " << mnist_bin_found << std::endl;
            }
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

    // If still no data, fall back to existing behaviour (this->all_data must have been set by tests)
    if (this->all_data == nullptr || this->all_data_length == 0) {
        std::cerr << "Error: no training data available (mnist or user drawings). Aborting training." << std::endl;
        std::exit(1);
    }

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