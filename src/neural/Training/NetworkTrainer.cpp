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

NetworkTrainer::NetworkTrainer() {
    // Default constructor for NetworkTrainer.
    // Initializes default training parameters and allocates a new
    // NetworkSettings instance which this trainer will own.
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
    
    this->current_learn_rate = this->network_settings->getInitialLearningRate();
    this->data_loaded = false;
    this->epoch = 0;
}

NetworkTrainer::NetworkTrainer(NetworkSettings* settings) {
    // Construct a trainer using a caller-provided NetworkSettings object.
    // Ownership: the caller retains ownership of "settings"; the trainer
    // will not delete it on destruction.
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

    this->current_learn_rate = this->network_settings->getInitialLearningRate();
    this->data_loaded = false;
    this->epoch = 0;
}
        
void NetworkTrainer::StartTrainingSession(int num_epochs) {
    // Start a training session that runs for "num_epochs" epochs.
    // Basically, it:
    //  1) Instantiate a NeuralNetwork configured from NetworkSettings.
    //  2) Ensure training data is loaded (calls LoadData() if needed).
    //  3) For each epoch: iterate the mini-batches and invoke Learn(...) to
    //     update the model parameters.
    //  4) After each epoch: evaluate on training and validation sets, print
    //     accuracies, shuffle batches, and decay the learning rate.
    //  5) At the end of training: evaluate and save the trained network if
    //     it improves on the stored checkpoint.


    // Initialize the neural network from settings.
    // NeuralNetwork takes ownership of the layer size array internally.
    NeuralNetwork* neural_network = new NeuralNetwork(this->network_settings->getLayerSizes(), this->network_settings->getNumLayers());
    neural_network->setActivationFunction(this->network_settings->getActivationType(), this->network_settings->getOutputActivationType());
    neural_network->set_cost_function(this->network_settings->getCostType());

    // Ensure training data is available before starting.
    if (!this->data_loaded) {
        this->LoadData();
        // If LoadData failed to find/trainable data it will leave data_loaded == false.
        if (!this->data_loaded) {
            std::cerr << "Training aborted: no training data available." << std::endl;
            // Load in a blank network of random values
            NetworkData network_data = NetworkData();
            network_data.SaveNetworkToSaved(neural_network, 0.0);
            delete neural_network;
            return;
        }
    }

    // Main training loop: run "num_epochs" epochs of learning.
    // Learning
    for (int epoch = 1; epoch <= num_epochs; epoch++) {
        // Process every mini-batch (training_batches was created by LoadData()).
        for (int batch = 0; batch < this->num_training_batches; batch++) {
            neural_network->Learn(this->training_batches[batch]->getData(), this->network_settings->getMiniBatchSize(), this->current_learn_rate, 
                                this->network_settings->getRegularisation(), this->network_settings->getMomentum());
        }

        // Update the public "epoch" counter to the current epoch index.
        this->epoch = epoch;

        // Evaluate performance on training and validation splits.
        // The returned EvaluationData objects must be deleted by the caller.
        EvaluationData* train_eval = NetworkEvaluator::Evaluate(neural_network, this->training_data, this->training_data_length);
        EvaluationData* validation_eval = NetworkEvaluator::Evaluate(neural_network, this->validation_data, this->validation_data_length);
        
        // Print a concise progress line with training/validation accuracies.
        std::cout << "Epoch " << this->epoch << ": Training Accuracy: " << train_eval->getNumCorrect() / (double)train_eval->getTotal() * 100.0 << "% Validation Accuracy: " <<
        validation_eval->getNumCorrect() / (double)validation_eval->getTotal() * 100.0 << "%" << std::endl;

        // Prepare for the next epoch: shuffle mini-batches and apply learning-rate decay.
        DatasetHandling::ShuffleBatches(this->training_batches, this->num_training_batches);
        this->current_learn_rate = (1.0 / (1.0 + this->network_settings->getLearnRateDecay() * this->epoch)) * this->network_settings->getInitialLearningRate();

        // Release evaluation objects for this epoch.
        delete train_eval;
        delete validation_eval;
    }

    // After finishing all epochs, compare with previously saved network (if any) and save the newly trained one.
    NetworkData network_data = NetworkData();
    NeuralNetwork* prev_network = network_data.LoadNetworkFromSaved();
    EvaluationData* validation_eval = NetworkEvaluator::Evaluate(neural_network, this->validation_data, this->validation_data_length);
    // Persist the trained network and its validation accuracy.
    network_data.SaveNetworkToSaved(neural_network, validation_eval->getNumCorrect() / (double)validation_eval->getTotal());
    
    // Cleanup local resources.
    delete prev_network;
    delete validation_eval;
    delete neural_network;
}

void NetworkTrainer::LoadData() {
    // LoadData populates "this->all_data" with examples to use for training.
    // Resolution order:
    //  1) Prefer an existing "data/dataset.bin" (pre-built/augmented dataset).
    //  2) If absent, look for user drawings ("data/user_drawings.bin" or
    //     "user_drawings.bin"), augment them and persist to "data/dataset.bin".
    //  3) If neither exist, leave "all_data" empty and return; callers should
    //     detect this and abort training rather than running on empty data.

    if (this->all_data == nullptr || this->all_data_length == 0) {
        const std::string dataset_path = "data/dataset.bin";
        // Ensure mnist.bin exists in data/: if it's missing, try to convert
        // common MNIST IDX files (gz or plain) into data/mnist.bin. This
        // step is independent of whether dataset.bin exists: we create the
        // mnist.bin so it's available for augmentation/versioning later.
        const std::string mnist_bin_path = "data/mnist.bin";
        std::ifstream f_mnist_check(mnist_bin_path, std::ios::binary);
        if (!f_mnist_check) {
            const std::vector<std::pair<std::string, std::string>> mnist_candidates = {
                {"data/train-images-idx3-ubyte.gz", "data/train-labels-idx1-ubyte.gz"},
                {"train-images-idx3-ubyte.gz", "train-labels-idx1-ubyte.gz"},
                {"data/train-images-idx3-ubyte", "data/train-labels-idx1-ubyte"},
                {"train-images-idx3-ubyte", "train-labels-idx1-ubyte"}
            };
            for (auto &p : mnist_candidates) {
                std::ifstream fi(p.first, std::ios::binary);
                std::ifstream fl(p.second, std::ios::binary);
                if (fi && fl) {
                    fi.close(); fl.close();
                    bool ok = MnistLoader::ConvertToBin(p.first, p.second, mnist_bin_path, CONSTANTS_H::output_layer_size);
                    if (ok) {
                        std::cout << "Converted MNIST IDX -> " << mnist_bin_path << "\n";
                    }
                    break;
                }
            }
        } else {
            // mnist.bin exists; load it briefly to report its size then free it.
            f_mnist_check.close();
            auto mn = DataSet::LoadDataPoints(mnist_bin_path);
            if (mn.second > 0 && mn.first != nullptr) {
                std::cout << "Loaded " << mn.second << " samples from " << mnist_bin_path << "\n";
                for (int i = 0; i < mn.second; ++i) delete mn.first[i];
                delete [] mn.first;
            }
        }
        // Check whether a prepared dataset exists first.
        std::ifstream f_dataset(dataset_path, std::ios::binary);
        if (f_dataset) {
            // dataset.bin exists: load it into memory.
            f_dataset.close();
            std::pair<DataPoint**, int> loaded = DataSet::LoadDataPoints(dataset_path);
            if (loaded.second > 0 && loaded.first != nullptr) {
                this->all_data = loaded.first;
                this->all_data_length = loaded.second;
                std::cout << "Loaded " << this->all_data_length << " samples from " << dataset_path << "\n";
            }
        } else {
            // dataset.bin not present: look for MNIST and/or user-submitted drawings,
            // augment them, and persist the combined augmented dataset as dataset.bin.

            DataPoint** user_arr = nullptr;
            int user_n = 0;
            DataPoint** mnist_arr = nullptr;
            int mnist_n = 0;

            // 1) user drawings
            const std::string user_path1 = "data/user_drawings.bin";
            const std::string user_path2 = "user_drawings.bin";
            std::string user_path_to_use;
            std::ifstream fu(user_path1, std::ios::binary);
            if (fu) { user_path_to_use = user_path1; fu.close(); }
            else {
                std::ifstream fu2(user_path2, std::ios::binary);
                if (fu2) { user_path_to_use = user_path2; fu2.close(); }
            }
            if (!user_path_to_use.empty()) {
                auto loaded_user = DataSet::LoadDataPoints(user_path_to_use);
                if (loaded_user.second > 0 && loaded_user.first != nullptr) {
                    user_arr = loaded_user.first;
                    user_n = loaded_user.second;
                    std::cout << "Loaded " << user_n << " samples from " << user_path_to_use << "\n";
                }
            }

            // 2) MNIST: prefer pre-converted data/mnist.bin, else try to convert IDX files
            const std::string mnist_bin1 = "data/mnist.bin";
            const std::string mnist_bin2 = "mnist.bin";
            std::string mnist_bin_to_use;
            std::ifstream fmb(mnist_bin1, std::ios::binary);
            if (fmb) { mnist_bin_to_use = mnist_bin1; fmb.close(); }
            else {
                std::ifstream fmb2(mnist_bin2, std::ios::binary);
                if (fmb2) { mnist_bin_to_use = mnist_bin2; fmb2.close(); }
            }

            if (mnist_bin_to_use.empty()) {
                // common locations for raw MNIST files (gz or plain)
                const std::vector<std::pair<std::string, std::string>> mnist_candidates = {
                    {"data/train-images-idx3-ubyte.gz", "data/train-labels-idx1-ubyte.gz"},
                    {"train-images-idx3-ubyte.gz", "train-labels-idx1-ubyte.gz"},
                    {"data/train-images-idx3-ubyte", "data/train-labels-idx1-ubyte"},
                    {"train-images-idx3-ubyte", "train-labels-idx1-ubyte"}
                };
                for (auto &p : mnist_candidates) {
                    std::ifstream fi(p.first, std::ios::binary);
                    std::ifstream fl(p.second, std::ios::binary);
                    if (fi && fl) {
                        fi.close(); fl.close();
                        const std::string out_bin = "data/mnist.bin";
                        bool ok = MnistLoader::ConvertToBin(p.first, p.second, out_bin, CONSTANTS_H::output_layer_size);
                        if (ok) {
                            mnist_bin_to_use = out_bin;
                            std::cout << "Converted MNIST IDX -> " << out_bin << "\n";
                        }
                        break;
                    }
                }
            }

            if (!mnist_bin_to_use.empty()) {
                auto loaded_mnist = DataSet::LoadDataPoints(mnist_bin_to_use);
                if (loaded_mnist.second > 0 && loaded_mnist.first != nullptr) {
                    mnist_arr = loaded_mnist.first;
                    mnist_n = loaded_mnist.second;
                    std::cout << "Loaded " << mnist_n << " samples from " << mnist_bin_to_use << "\n";
                }
            }

            // 3) If we have any source data, augment and combine
            if ((user_n > 0 && user_arr != nullptr) || (mnist_n > 0 && mnist_arr != nullptr)) {
                std::vector<DataPoint*> all_augmented;

                auto augment_and_append = [&](DataPoint** arr, int n) {
                    for (int i = 0; i < n; ++i) {
                        int width = (arr[i]->getInputsLength() == CONSTANTS_H::DESTX * CONSTANTS_H::DESTY) ? CONSTANTS_H::DESTX : 28;
                        int height = (arr[i]->getInputsLength() == CONSTANTS_H::DESTX * CONSTANTS_H::DESTY) ? CONSTANTS_H::DESTY : 28;
                        std::vector<DataPoint*> aug = Augmentations::GenerateAugmentedDataPoints(arr[i]->getInputs(), width, height, arr[i]->getLabel(), CONSTANTS_H::output_layer_size);
                        all_augmented.insert(all_augmented.end(), aug.begin(), aug.end());
                    }
                };

                if (user_n > 0 && user_arr != nullptr) augment_and_append(user_arr, user_n);
                if (mnist_n > 0 && mnist_arr != nullptr) augment_and_append(mnist_arr, mnist_n);

                for (int i = 0; i < user_n; ++i) {
                    std::vector<DataPoint*> aug = Augmentations::GenerateAugmentedDataPoints(user_arr[i]->getInputs(), user_arr[i]->getInputsLength() == CONSTANTS_H::DESTX * CONSTANTS_H::DESTY ? CONSTANTS_H::DESTX : 28, user_arr[i]->getInputsLength() == CONSTANTS_H::DESTX * CONSTANTS_H::DESTY ? CONSTANTS_H::DESTY : 28, user_arr[i]->getLabel(), CONSTANTS_H::output_layer_size);
                    all_augmented.insert(all_augmented.end(), aug.begin(), aug.end());
                }
                std::cout << "Saving augmented dataset to " << dataset_path << " (" << all_augmented.size() << " datapoints)...\n";
                DataSet::SaveDataPoints(all_augmented.data(), all_augmented.size(), dataset_path);

                // Cleanup original loaded sources
                if (user_arr) {
                    for (int i = 0; i < user_n; ++i) delete user_arr[i];
                    delete[] user_arr;
                }
                if (mnist_arr) {
                    for (int i = 0; i < mnist_n; ++i) delete mnist_arr[i];
                    delete[] mnist_arr;
                }

                // Load the newly-saved combined dataset into memory for training
                std::pair<DataPoint**, int> loaded_aug = DataSet::LoadDataPoints(dataset_path);
                if (loaded_aug.second > 0 && loaded_aug.first != nullptr) {
                    this->all_data = loaded_aug.first;
                    this->all_data_length = loaded_aug.second;
                    std::cout << "Loaded " << this->all_data_length << " samples from " << dataset_path << "\n";
                }

                // Free transient augmented DataPoint pointers created by GenerateAugmentedDataPoints
                for (auto dp : all_augmented) delete dp;
            }
        }
    }

    // If still no data was found, set data_loaded=false and return; the
    // StartTrainingSession caller checks this flag and will abort training
    // gracefully rather than proceeding with an empty dataset.
    if (this->all_data == nullptr || this->all_data_length == 0) {
        std::cerr << "Error: no training data available (mnist or user drawings). Aborting training." << std::endl;
        this->data_loaded = false;
        return;
    }

    // Split the loaded dataset into training/validation and create minibatches.
    std::pair<std::pair<DataPoint**, int>, std::pair<DataPoint**, int>> result = DatasetHandling::SplitData(this->all_data, this->all_data_length, this->training_split);
    this->training_data = result.first.first;
    this->training_data_length = result.first.second;
    this->validation_data = result.second.first;
    this->validation_data_length = result.second.second;

    std::pair<Batch**, int> result1 = DatasetHandling::CreateMiniBatches(this->training_data, this->training_data_length, this->network_settings->getMiniBatchSize());
    this->training_batches = result1.first;
    this->num_training_batches = result1.second;
    this->data_loaded = true;
}

// Internal testing helpers
// These helpers are intended for unit tests and debugging. They allow
// injecting dataset pointers directly; callers remain responsible for
// ownership and cleanup of any memory passed in.
void NetworkTrainer::_set_data(DataPoint** data, int data_length) {
    this->all_data = data;
    this->all_data_length = data_length;
}

NetworkTrainer::~NetworkTrainer() {
    if (this->defaultConstructed) {
        delete this->network_settings;
    }
}