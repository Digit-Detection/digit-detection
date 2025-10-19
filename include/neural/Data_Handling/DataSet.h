#pragma once
#include "neural/Data_Handling/DataPoint.h"
#include <string>
#include <utility>

// Functions for saving/loading DataPoint records to/from a binary file.
// File format (record stream, no global header):
// [int inputs_length][int label][int num_labels][double inputs[inputs_length]] ... repeated
class DataSet {
public:
    // Save (truncate) an array of datapoints to a file
    static void SaveDataPoints(DataPoint** data, int data_length, const std::string& filename);

    // Append a single datapoint record to the file
    static void AppendDataPoint(DataPoint* dp, const std::string& filename);

    // Load all datapoints from file. Returns pair of (array, length).
    // Caller owns the returned DataPoint** array and each DataPoint inside it.
    static std::pair<DataPoint**, int> LoadDataPoints(const std::string& filename);
};
