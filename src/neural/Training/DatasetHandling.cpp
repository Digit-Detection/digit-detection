#include "neural/Training/DatasetHandling.h"
#include <utility>
#include <algorithm>
#include <random>

std::pair<std::pair<DataPoint**, int>, std::pair<DataPoint**, int>> DatasetHandling::SplitData(DataPoint** all_data, int all_data_length, float training_split, bool shuffle) {
    // Splits sample data into training data and testing data for the neural network
    if (shuffle) {
        this->ShuffleArray(all_data, all_data_length);
    }
    if (training_split > 1.0) {
        training_split = 1.0;
    }
    else if (training_split < 0.0) {
        training_split = 0.0;
    }
    // Counts of the arrays
    int train_count = (int)(all_data_length * training_split);
    int validation_count = all_data_length - train_count;

    DataPoint** train_data = new DataPoint*[train_count];
    DataPoint** validation_data = new DataPoint*[validation_count];

    // Putting values into the array
    for (int i = 0; i < train_count; i++) {
        train_data[i] = all_data[i];
    }
    for (int i = 0; i < validation_count; i++) {
        validation_data[i] = all_data[i + train_count];
    }
    return std::make_pair(std::make_pair(train_data, train_count), std::make_pair(validation_data, validation_count));
}
        
std::pair<Batch**, int> DatasetHandling::CreateMiniBatches(DataPoint** all_data, int all_data_length, int size, bool shuffle) {
    // Creates Batches for training data, this can optimise training
    if (shuffle) {
        this->ShuffleArray(all_data, all_data_length);
    }

    int num_batches = all_data_length / size;
    Batch** batches = new Batch*[num_batches];

    // Inserting data into batches
    for (int i = 0; i < num_batches; i++) {
        DataPoint** batch_data = new DataPoint*[size];
        for (int j = 0; j < size; j++) {
            batch_data[j] = all_data[i * size + j];
        }
        batches[i] = new Batch(batch_data, size);
    }
    return std::make_pair(batches, num_batches);
}
        
void DatasetHandling::ShuffleBatches(Batch** batches, int num_batches) {
    this->ShuffleArray(batches, num_batches);
}

template <typename T>
void DatasetHandling::ShuffleArray(T** array, int array_length) {
    // Shuffles the array like a deck of cards
    std::default_random_engine rng;
    std::shuffle(array, array + array_length, rng);
}