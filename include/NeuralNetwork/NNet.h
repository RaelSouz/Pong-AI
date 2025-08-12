#pragma once

#include <NeuralNetwork/mat.h>
#include <cmath>

class NNet {
    public:
        NNet();

        const float&  feedFoward(vec& input);
        void backPropagation(float target, float learnRate);

    private:
        mat layer1;
        vec layer2;
        vec in;
        vec outL1;
        float outL2;
};

namespace nnet {
    float Tanh(float x, bool derivate = false);
    float normalizeTanh(float value, float min, float max);
}