#include "neural/Training/EvaluationData.h"

EvaluationData::EvaluationData(int num_labels) {
    this->num_labels = num_labels;
    this->num_correct = 0;
    this->total = 0;
    this->num_correct_per_label = new int[num_labels];
    this->total_per_label = new int[num_labels];
    this->falsely_labelled = new int[num_labels];
    for (int i = 0; i < num_labels; i++) {
        this->num_correct_per_label[i] = 0;
        this->total_per_label[i] = 0;
        this->falsely_labelled[i] = 0;
    }
}

int EvaluationData::get_num_correct() {
    return this->num_correct;
}

void EvaluationData::increment_num_correct() {
    this->num_correct++;
}
        
int EvaluationData::get_total() {
    return this->total;
}

void EvaluationData::set_total(int total) {
    this->total = total;
}
        
int* EvaluationData::get_num_correct_per_label() {
    return this->num_correct_per_label;
}
        
int* EvaluationData::get_total_per_label() {
    return this->total_per_label;
}
        
int* EvaluationData::get_falsely_labelled() {
    return this->falsely_labelled;
}
        
void EvaluationData::increment_num_correct_per_label(int index) {
    this->num_correct_per_label[index]++;
}
        
void EvaluationData::increment_total_per_label(int index) {
    this->total_per_label[index]++;
}
        
void EvaluationData::increment_falsely_labelled(int index) {
    this->falsely_labelled[index]++;
}

EvaluationData::~EvaluationData() {
    delete[] this->num_correct_per_label;
    delete[] this->total_per_label;
    delete[] this->falsely_labelled;
}