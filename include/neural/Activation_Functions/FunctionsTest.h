#pragma once
#include "neural/Activation_Functions/CallActivation.h"
#include "neural/Activation_Functions/CallCost.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

class FunctionsTest {
public:
    void runTests() {
        sigmoid_test();
        sigmoid_derivative_test();
        softmax_test();
        softmax_derivative_test();
        crossEntropy_test();
        crossEntropy_derivative_test();
        mse_test();
        mse_derivative_test();
        std::cout << "\nAll tests passed successfully!" << std::endl;
    }

private:
    // Helper to compare doubles
    bool approxEqual(double a, double b, double range = 1e-6) {
        return std::fabs(a - b) < range;
    }

    void sigmoid_test() {
        std::cout << "Running sigmoid test..." << std::endl;
        CallActivation activation(ActivationType::sigmoid);
        Activations* func = activation.get_activation();

        std::vector<double> inputs = {0.0, 1.0, -1.0};
        int n = (int)inputs.size();

        for (int i = 0; i < n; ++i) {
            double out = func->Activate(inputs.data(), n, i);
            double expected = 1.0 / (1.0 + std::exp(-inputs[i]));
            assert(approxEqual(out, expected));
        }

        std::cout << "sigmoid test passed" << std::endl;
    }

    
    void sigmoid_derivative_test() {
        std::cout << "Running sigmoid_derivative test..." << std::endl;
        CallActivation activation(ActivationType::sigmoid);
        Activations* func = activation.get_activation();

        std::vector<double> inputs = {0.0, 1.0, -1.0};
        int n = (int)inputs.size();

        for (int i = 0; i < n; ++i) {
            double s = 1.0 / (1.0 + std::exp(-inputs[i]));
            double expected = s * (1.0 - s);
            double actual = func->Der(inputs.data(), n, i);
            assert(approxEqual(actual, expected));
        }

        std::cout << "sigmoid_derivative test passed" << std::endl;
    }

    void softmax_test() {
        std::cout << "Running softmax test..." << std::endl;
        CallActivation activation(ActivationType::softmax);
        Activations* func = activation.get_activation();

        std::vector<double> inputs = {1.0, 2.0, 3.0};
        int n = (int)inputs.size();

        double sumExp = 0.0;
        for (double v : inputs) sumExp += std::exp(v);

        for (int i = 0; i < n; ++i) {
            double out = func->Activate(inputs.data(), n, i);
            double expected = std::exp(inputs[i]) / sumExp;
            assert(approxEqual(out, expected));
        }

        std::cout << "softmax test passed" << std::endl;
    }

    void softmax_derivative_test() {
        std::cout << "Running softmax_derivative test..." << std::endl;
        CallActivation activation(ActivationType::softmax);
        Activations* func = activation.get_activation();

        std::vector<double> inputs = {1.0, 2.0, 3.0};
        int n = (int)inputs.size();

        // Compute softmax outputs
        std::vector<double> softmaxVals(n);
        double sumExp = 0.0;
        for (double v : inputs) sumExp += std::exp(v);
        for (int i = 0; i < n; ++i)
            softmaxVals[i] = std::exp(inputs[i]) / sumExp;

        // The derivative of softmax(i) w.r.t itself (diagonal term)
        // dS_i/dZ_i = S_i * (1 - S_i)
        for (int i = 0; i < n; ++i) {
            double expected = softmaxVals[i] * (1.0 - softmaxVals[i]);
            double actual = func->Der(inputs.data(), n, i);
            assert(approxEqual(actual, expected));
        }

        std::cout << "softmax_derivative test passed" << std::endl;
    }

    void crossEntropy_test() {
        std::cout << "Running crossEntropy test..." << std::endl;
        CallCost cost(CostType::crossEntropy);
        Costs* func = cost.get_activation();

        std::vector<double> y_true = {0.0, 1.0, 0.0};
        std::vector<double> y_pred = {0.1, 0.8, 0.1};
        int n = (int)y_true.size();

        double expected = -std::log(1 - 0.1) - std::log(0.8) - std::log(1 - 0.1);

        double actual = func->Output(y_pred.data(), y_true.data(), n);

        // std::cout << "  Expected: " << expected << "  Actual: " << actual << std::endl;
        assert(approxEqual(actual, expected));

        std::cout << "crossEntropy test passed" << std::endl;
    }

    void crossEntropy_derivative_test() {
        std::cout << "Running crossEntropy_derivative test..." << std::endl;
        CallCost cost(CostType::crossEntropy);
        Costs* func = cost.get_activation();

        // Test data (binary targets + sigmoid-like outputs)
        double y_true[] = {1.0, 0.0, 1.0, 0.0};
        double y_pred[] = {0.9, 0.1, 0.8, 0.2};
        int n = 4;

        // Expected derivative for binary cross entropy:
        // dL/dx = (x - y) / (x * (1 - x))
        for (int i = 0; i < n; i++) {
            double x = y_pred[i];
            double y = y_true[i];

            // avoid division by zero for edge values
            if (x == 0.0 || x == 1.0)
                continue;

            double expected = (x - y) / (x * (1 - x));
            double actual = func->Der(x, y);

            assert(approxEqual(actual, expected));
        }

        std::cout << "crossEntropy_derivative test passed" << std::endl;
    }

    void mse_test() {
        std::cout << "Running mse test..." << std::endl;
        CallCost cost(CostType::meanSquaredError);
        Costs* func = cost.get_activation();

        double y_true[] = {1.0, 0.0, 0.0};
        double y_pred[] = {0.8, 0.1, 0.1};
        int n = 3;

        // Expected: 0.5 * Σ((x - y)^2)
        double expected = 0.0;
        for (int i = 0; i < n; i++) {
            double diff = y_pred[i] - y_true[i];
            expected += diff * diff;
        }
        expected *= 0.5;

        double actual = func->Output(y_pred, y_true, n);

        //std::cout << "  Expected: " << expected << "  Actual: " << actual << "\n";
        assert(approxEqual(actual, expected));

        std::cout << "mse test passed" << std::endl;
    }

    void mse_derivative_test() {
        std::cout << "Running mse_derivative test..." << std::endl;
        CallCost cost(CostType::meanSquaredError);
        Costs* func = cost.get_activation();

        double y_true[] = {1.0, 0.0, 0.5};
        double y_pred[] = {0.8, 0.1, 0.4};
        int n = 3;

        // Expected derivative: dL/dx = (x - y)
        for (int i = 0; i < n; i++) {
            double x = y_pred[i];
            double y = y_true[i];
            double expected = x - y;
            double actual = func->Der(x, y);

            assert(approxEqual(actual, expected));
        }


        std::cout << "mse_derivative test passed" << std::endl;
    }
};
