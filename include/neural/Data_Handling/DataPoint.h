#ifndef DATAPOINT_H
#define DATAPOINT_H

class DataPoint {
    private:
        double* inputs;
        int inputs_length;
        double* expected_outputs;
        int expected_outputs_length;
        int label;
    public:
        DataPoint(double* inputs, int inputs_length, int label, int num_labels);
        double* getInputs();
        int getInputsLength();
        int getLabel();
        double* getExpectedOutputs();
        int getExpectedOutputsLength();
        double* CreateOneHot(int index, int num);
        ~DataPoint();
};

#endif