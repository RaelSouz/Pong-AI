#pragma once

#include <vector>
#include <cassert>
#include <stdexcept>
#include <random>

class vec {
    public:
        vec(size_t size);
        vec(const vec& outra) = default;
        vec(const std::vector<float>& values);
        vec(std::vector<float>::const_iterator begin, std::vector<float>::const_iterator end);

        size_t size() const;
        vec& operator=(const vec& rhs) = default;
        vec& operator=(vec&& rhs) noexcept;
        float& operator[](size_t index);
        const float& operator[](size_t index) const;
        vec operator*(const vec& rhs) const;
        float sum();
        

    private:
        std::vector<float> dt;
};

namespace nnet {
    namespace random {
        vec uniform(size_t size, float min, float max);
    }
}