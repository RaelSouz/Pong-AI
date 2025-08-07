#pragma once

#include <vector>
#include <cassert>
#include <stdexcept>
#include <random>

class vec {
    public:
        vec(size_t size);
        vec(const vec& outra) = default;
        vec(const std::vector<double>& values);
        vec(std::vector<double>::const_iterator begin, std::vector<double>::const_iterator end);

        size_t size() const;
        vec& operator=(const vec& rhs) = default;
        vec& operator=(vec&& rhs) noexcept;
        double& operator[](size_t index);
        const double& operator[](size_t index) const;
        vec operator*(const vec& rhs) const;
        double sum();
        

    private:
        std::vector<double> dt;
};

namespace nnet {
    namespace random {
        vec uniform(size_t size, double min, double max);
    }
}