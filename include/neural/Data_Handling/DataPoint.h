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
        double* get_inputs();
        int get_inputs_length();
        int get_label();
        double* get_expected_outputs();
        int get_expected_outputs_length();
        double* CreateOneHot(int index, int num);
        ~DataPoint();
};

#endif