#include "neural/Activation_Functions/CallCost.h"
#include "neural/Activation_Functions/CrossEntropy.h"
#include "neural/Activation_Functions/MeanSquaredError.h"

CallCost::CallCost(CostType activation) {
    this->set_activation(activation);
}

Costs* CallCost::get_activation() {
    return this->activation;
}

void CallCost::set_activation(CostType activation) {
    switch (activation) {
        case meanSquaredError:
            this->activation = new MeanSquaredError();
            break;
        case crossEntropy:
            this->activation = new CrossEntropy();
            break;
        default:
            this->activation = new MeanSquaredError();
            break;
    }
}

CallCost::~CallCost() {
    delete this->activation;
}