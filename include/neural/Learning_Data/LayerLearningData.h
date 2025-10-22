#ifndef LAYERLEARNINGDATA_H
#define LAYERLEARNINGDATA_H

//Forward Declaration to prevent circular dependency
class Layer;

class LayerLearningData {
    private:
        int size;
        double* inputs;
        double* weighted_inputs;
        double* activations;
        double* node_values;
    public:
        LayerLearningData();
        LayerLearningData(Layer* layer);
        void setInputs(double* inputs);
        void setWeightedInputs(int index, double value);
        double* getWeightedInputs();
        int getSize();
        void setActivations(int index, double value);
        double* getActivations();
        void setNodeValues(int index, double value);
        double* getNodeValues();
        double getNodeValues(int index);
        double getInputs(int index);
        ~LayerLearningData();
};

#endif