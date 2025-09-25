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
        void set_inputs(double* inputs);
        void set_weighted_inputs(int index, double value);
        double* get_weighted_inputs();
        int get_size();
        void set_activations(int index, double value);
        double* get_activations();
        void set_node_values(int index, double value);
        double* get_node_values();
        double get_node_values(int index);
        double get_inputs(int index);
        ~LayerLearningData();
};

#endif