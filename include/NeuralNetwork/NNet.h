#pragma once

#include <NeuralNetwork/mat.h>
#include <cmath>

class NNet {
    public:
        NNet();

        const double&  feedFoward(vec& input);
        void backPropagation(double target, double learnRate);

    private:
        mat layer1;
        vec layer2;
        vec in;
        vec outL1;
        double outL2;
};

namespace nnet {
    double Tanh(double x, bool derivate = false);
    double normalizeTanh(double value, double min, double max);
}