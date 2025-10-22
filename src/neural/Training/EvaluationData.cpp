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

int EvaluationData::getNumCorrect() {
    return this->num_correct;
}

void EvaluationData::incrementNumCorrect() {
    this->num_correct++;
}
        
int EvaluationData::getTotal() {
    return this->total;
}

void EvaluationData::setTotal(int total) {
    this->total = total;
}
        
int* EvaluationData::getNumCorrectPerLabel() {
    return this->num_correct_per_label;
}
        
int* EvaluationData::getTotalPerLabel() {
    return this->total_per_label;
}
        
int* EvaluationData::getFalselyLabelled() {
    return this->falsely_labelled;
}
        
void EvaluationData::incrementNumCorrectPerLabel(int index) {
    this->num_correct_per_label[index]++;
}
        
void EvaluationData::incrementTotalPerLabel(int index) {
    this->total_per_label[index]++;
}
        
void EvaluationData::incrementFalselyLabelled(int index) {
    this->falsely_labelled[index]++;
}

EvaluationData::~EvaluationData() {
    delete[] this->num_correct_per_label;
    delete[] this->total_per_label;
    delete[] this->falsely_labelled;
}