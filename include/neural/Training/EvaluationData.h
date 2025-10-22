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
        int getNumCorrect();
        void incrementNumCorrect();
        int getTotal();
        void setTotal(int total);
        int* getNumCorrectPerLabel();
        int* getTotalPerLabel();
        int* getFalselyLabelled();
        void incrementNumCorrectPerLabel(int index);
        void incrementTotalPerLabel(int index);
        void incrementFalselyLabelled(int index);
        ~EvaluationData();
};

#endif