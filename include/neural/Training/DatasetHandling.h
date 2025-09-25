#ifndef DATASETHANDLING_H
#define DATASETHANDLING_H
#include "neural/Training/Batch.h"
#include <utility>

// Class for Dataset preparation for training
class DatasetHandling {
    public:
        std::pair<std::pair<DataPoint**, int>, std::pair<DataPoint**, int>> SplitData(DataPoint** all_data, int all_data_length, float training_split = 0.75, bool shuffle = true);
        std::pair<Batch**, int> CreateMiniBatches(DataPoint** all_data, int all_data_length, int size, bool shuffle = true);
        void ShuffleBatches(Batch** batches, int num_batches);
        template <typename T>
        static void ShuffleArray(T** array, int array_length);
};

#endif