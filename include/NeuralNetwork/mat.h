#pragma once

#include <NeuralNetwork/vec.h>

class mat {
    public:
        mat(size_t rows, size_t columns);

        size_t size() const;
        size_t rows() const;
        size_t columns() const;
        double& operator[](size_t index);
        const double& operator[](size_t index) const;
        vec operator()(size_t index);
        double& operator()(size_t indexY, size_t indexX);
        const double& operator()(size_t indexY, size_t indexX) const;

    private:
        std::vector<double> dt;
        size_t _rows, _columns, _strY, _strX;
};

namespace nnet {
    namespace random {
        mat uniform(size_t rows, size_t columns, double min, double max);
    }
}