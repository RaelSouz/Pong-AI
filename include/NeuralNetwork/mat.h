#pragma once

#include <NeuralNetwork/vec.h>

class mat {
    public:
        mat(size_t rows, size_t columns);

        size_t size() const;
        size_t rows() const;
        size_t columns() const;
        float& operator[](size_t index);
        const float& operator[](size_t index) const;
        vec operator()(size_t index);
        float& operator()(size_t indexY, size_t indexX);
        const float& operator()(size_t indexY, size_t indexX) const;

    private:
        std::vector<float> dt;
        size_t _rows, _columns;
};

namespace nnet {
    namespace random {
        mat uniform(size_t rows, size_t columns, float min, float max);
    }
}