#include <utility>
#include <iostream>
#include "neural/Training/NetworkTrainer.h"
#include "neural/Network/NeuralNetwork.h"
#include "neural/Network/NetworkSettings.h"
#include "neural/Training/DatasetHandling.h"
#include "neural/Data_Handling/NetworkData.h"
#include "neural/Data_Handling/MnistLoader.h"
#include "neural/Data_Handling/DataSet.h"
#include "neural/Data_Handling/Augmentations.h"
#include "constants.h"
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
    // Check for data/dataset.bin first. If it exists we'll load it directly.
    // If it does not exist, we will load `data/user_drawings.bin` (if present),
    // generate augmentations and call DataSet::SaveDataPoints(...) to create `data/dataset.bin`.
    const std::string dataset_path = "data/dataset.bin";
        std::ifstream f_dataset(dataset_path, std::ios::binary);
        if (f_dataset) {
            f_dataset.close();
            std::pair<DataPoint**, int> loaded = DataSet::LoadDataPoints(dataset_path);
            if (loaded.second > 0 && loaded.first != nullptr) {
                this->all_data = loaded.first;
                this->all_data_length = loaded.second;
                std::cout << "Loaded " << this->all_data_length << " samples from " << dataset_path << "\n";
            }
        } else {
            // If not present, load user_drawings.bin and augment
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
            // Augment all user drawings and write to dataset.bin
            if (user_n > 0 && user_arr != nullptr) {
                std::vector<DataPoint*> all_augmented;
                for (int i = 0; i < user_n; ++i) {
                    std::vector<DataPoint*> aug = Augmentations::GenerateAugmentedDataPoints(user_arr[i]->get_inputs(), user_arr[i]->get_inputs_length() == CONSTANTS_H::DESTX * CONSTANTS_H::DESTY ? CONSTANTS_H::DESTX : 28, user_arr[i]->get_inputs_length() == CONSTANTS_H::DESTX * CONSTANTS_H::DESTY ? CONSTANTS_H::DESTY : 28, user_arr[i]->get_label(), CONSTANTS_H::NUMDIGITS);
                    all_augmented.insert(all_augmented.end(), aug.begin(), aug.end());
                }
                std::cout << "Saving augmented dataset to " << dataset_path << " (" << all_augmented.size() << " datapoints)...\n";
                // SaveDataPoints opens the file with truncation and will create `data/dataset.bin` and its parent directories if needed.
                DataSet::SaveDataPoints(all_augmented.data(), all_augmented.size(), dataset_path);
                // Clean up user_arr
                for (int i = 0; i < user_n; ++i) delete user_arr[i];
                delete[] user_arr;
                // Clean up augmented DataPoints after loading
                std::pair<DataPoint**, int> loaded_aug = DataSet::LoadDataPoints(dataset_path);
                if (loaded_aug.second > 0 && loaded_aug.first != nullptr) {
                    this->all_data = loaded_aug.first;
                    this->all_data_length = loaded_aug.second;
                    std::cout << "Loaded " << this->all_data_length << " samples from " << dataset_path << "\n";
                }
                for (auto dp : all_augmented) delete dp;
            }
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
    if (this->defaultConstructed) {
        delete this->network_settings;
    }
}