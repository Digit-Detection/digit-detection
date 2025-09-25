#ifndef EVALUATIONDATA_H
#define EVALUATIONDATA_H

class EvaluationData {
    private:
        int num_correct;
        int total;

        int* num_correct_per_label;
        int* total_per_label;
        int* falsely_labelled;
        int num_labels;
    public:
        EvaluationData(int num_labels);
        int get_num_correct();
        void increment_num_correct();
        int get_total();
        void set_total(int total);
        int* get_num_correct_per_label();
        int* get_total_per_label();
        int* get_falsely_labelled();
        void increment_num_correct_per_label(int index);
        void increment_total_per_label(int index);
        void increment_falsely_labelled(int index);
        ~EvaluationData();
};

#endif