#include "neural/Data_Handling/DataPoint.h"

DataPoint::DataPoint(double* inputs, int inputs_length, int label, int num_labels) {
    //Shallow copy is bad, use deep copy
    this->inputs = new double[inputs_length];
    for (int i = 0; i < inputs_length; i++) {
        this->inputs[i] = inputs[i];
    }
    this->inputs_length = inputs_length;
    this->label = label;
    this->expected_outputs_length = num_labels;
    this->expected_outputs = this->CreateOneHot(label, num_labels);
}

double* DataPoint::get_inputs() {
    return this->inputs;
}

int DataPoint::get_inputs_length() {
    return this->inputs_length;
}

int DataPoint::get_label() {
    return this->label;
}

double* DataPoint::get_expected_outputs() {
    return this->expected_outputs;
}

int DataPoint::get_expected_outputs_length() {
    return this->expected_outputs_length;
}

double* DataPoint::CreateOneHot(int index, int num) {
    // Creates expected outputs array from one correct label vs all other incorrect labels,
    // setting for example: label=3, num_labels=5 -> output: [0,0,0,1,0]
    double* one_hot = new double[num];
    for (int i = 0; i < num; i++) {
        one_hot[i] = (i == index) ? 1.0 : 0.0; // Initialise all elements
    }
    return one_hot;
}

DataPoint::~DataPoint() {
    delete[] this->inputs;
    delete[] this->expected_outputs;
}