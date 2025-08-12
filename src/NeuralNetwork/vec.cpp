#include <NeuralNetwork/vec.h>

vec::vec(size_t size) : dt(size, 0.0f) {}

vec::vec(const std::vector<float>& values) {
    dt = std::move(values);
}

vec::vec(std::vector<float>::const_iterator begin, std::vector<float>::const_iterator end) :
    dt(begin, end)
{}

size_t vec::size() const {
    return dt.size();
}

vec& vec::operator=(vec&& rhs) noexcept {
    if(this != &rhs) {
        dt = std::move(rhs.dt);
    }
    return *this;
}

float& vec::operator[](size_t index) {
    if(index >= dt.size()) throw std::out_of_range("Index out of range");
    return dt[index];
}

const float& vec::operator[](size_t index) const {
    if(index >= dt.size()) throw std::out_of_range("Index out of range");
    return dt[index];
}

vec vec::operator*(const vec& rhs) const {
    assert(dt.size() == rhs.size());
    vec v(dt.size());
    for(int i = 0; i < dt.size(); i++) {
        v[i] = dt[i] * rhs[i];
    }
    return v;
}

float vec::sum() {
    float s = 0.0f;
    for(float value : dt) s += value;
    return s;
}

vec nnet::random::uniform(size_t size, float min, float max) {
    std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> d(min, max);
	vec v = vec(size);
	for(int i = 0; i < v.size(); i++) {
        v[i] = d(gen);
    }
	return v;
}