#include <NeuralNetwork/mat.h>

mat::mat(size_t rows, size_t columns) : 
    dt(rows * columns),
    _rows(rows), 
    _columns(columns) {}

size_t mat::size() const {
    return dt.size();
}

size_t mat::rows() const {
    return _rows;
}

size_t mat::columns() const {
    return _columns;
}

double& mat::operator[](size_t index) {
    if(index >= dt.size()) throw std::out_of_range("Index out of range");
    return dt[index];
}

const double& mat::operator[](size_t index) const {
    if(index >= dt.size()) throw std::out_of_range("Index out of range");
    return dt[index];
}

double& mat::operator()(size_t indexY, size_t indexX) {
    assert(indexY < _rows && indexX < _columns);
	return dt[(indexY * (_columns + _strX)) + indexX];
}

vec mat::operator()(size_t index) {
    assert(index < _rows);
    size_t beginRow = (index * (_columns + _strX));
    return vec((dt.begin() + beginRow), (dt.begin() + beginRow + _columns));
}

const double& mat::operator()(size_t indexY, size_t indexX) const {
    assert(indexY < _rows && indexX < _columns);
	return dt[(indexY * (_columns + _strX)) + indexX];
}

mat nnet::random::uniform(size_t rows, size_t columns, double min, double max) {
    std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> d(min, max);
	mat m = mat(rows, columns);
	for(int i = 0; i < m.size(); i++) {
        m[i] = d(gen);
    }
	return m;
}