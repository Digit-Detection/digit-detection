#ifndef BATCH_H
#define BATCH_H
#include "neural/Data_Handling/DataPoint.h"

class Batch {
    private:
        DataPoint** data;
        int data_length;
    public:
        Batch(DataPoint** data, int data_length);
        DataPoint** get_data();
        ~Batch();
};

#endif