#include <NeuralNetwork/NNet.h>

NNet::NNet() : 
    layer1(nnet::random::uniform(6, 3, -0.5f, 0.5f)),
    layer2(nnet::random::uniform(6, -0.5f, 0.5f)),
    in(3),
    outL1(6),
    outL2(0.0f)
    {}

const float& NNet::feedFoward(vec& input) {
    in = input;
    int i;
    for(i = 0; i < outL1.size(); i++) {
        outL1[i] = nnet::Tanh((in * layer1(i)).sum());
    }

    outL2 = nnet::Tanh((outL1 * layer2).sum());
    return outL2;
}

void NNet::backPropagation(float target, float learnRate) {
    int i, j;
    float loss = (outL2 - target) * nnet::Tanh(outL2, true);

    for(i = 0; i < layer2.size(); i++) {
        layer2[i] -= learnRate * loss * outL1[i];
    }

    for(i = 0; i < layer1.rows(); i++) {
        for(j = 0; j < layer1.columns(); j++) {
            layer1(i, j) -= learnRate * loss * in[j];
        }
    }
}

float nnet::Tanh(float x, bool derivate) {
    return derivate ? 1.0f - x * x : static_cast<float>(tanh(x));
}

float nnet::normalizeTanh(float value, float min, float max) {
    return 2.0f * ((value - min) / (max - min)) - 1.0f;
}