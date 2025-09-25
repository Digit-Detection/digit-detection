#include <iostream>
#include "neural/Training/NetworkTrainer.h"

class NetworkTest {
    public:
        void runTests() {
            testSimpleData();
        }
    private:
        void testSimpleData() {
            NetworkSettings* settings = new NetworkSettings();
            // ARCHITECTURE
            int* layer_sizes = new int[3];
            layer_sizes[0] = 2;   // input dimension
            layer_sizes[1] = 8;   // hidden layer
            layer_sizes[2] = 2;   // output layer (two classes)

            // Configure Settings
            settings->set_layer_sizes(layer_sizes);
            settings->set_num_layers(3);
            NetworkTrainer network_trainer(settings); // Create trainer

            const int N = 200;
            DataPoint** data = new DataPoint*[N];

            // Splitting of 25 Negatives (label 0)
            for (int i = 0; i < N/2; ++i) {
                double* inputs = new double[2];
                // Sample values that SHOULD give label 0
                // roughly centered and normalized to [-1,1]
                inputs[0] = ((double)rand() / RAND_MAX) * 0.8 - 0.4;
                inputs[1] = ((double)rand() / RAND_MAX) * 0.8 - 0.4;
                data[i] = new DataPoint(inputs, 2, 0, 2);
                delete[] inputs;
            }

            // 25 Positives (label 1)
            for (int i = N/2; i < N; ++i) {
                double* inputs = new double[2];
                // Sample values that SHOULD give label 1
                inputs[0] = ((double)rand() / RAND_MAX) * 0.8 + 0.5;
                inputs[1] = ((double)rand() / RAND_MAX) * 0.8 + 0.5;
                data[i] = new DataPoint(inputs, 2, 1, 2);
                delete[] inputs;
            }

            // Load data and train
            network_trainer._set_data(data, N);
            network_trainer.LoadData();
            network_trainer.StartTrainingSession(20);

            // ----- Clean up -----
            for (int i = 0; i < N; ++i) {
                delete data[i];
            }
            delete[] data;
            delete[] layer_sizes;
        }

};

