#include "dynamic_tensor.h"

#include <vector>
#include <cassert>

// Default constructor
DynamicTensor::DynamicTensor() {}

// Constructor with shape
DynamicTensor::DynamicTensor(const std::vector<size_t>& input_shape, double initial_value)
    : shape_(input_shape) {
    size_t total_size = 1;
    if (!shape_.empty()) {
        for (size_t dim : shape_) total_size *= dim;
    } else {
        total_size = 0;
    }
    data_.assign(total_size, initial_value);
}

// Constructor with shape and data
DynamicTensor::DynamicTensor(const std::vector<size_t>& input_shape, const std::vector<double>& data)
    : data_(data), shape_(input_shape) {
    size_t required_size = 1;
    if (!shape_.empty()) {
        for (size_t dim : shape_) required_size *= dim;
    } else {
        required_size = 0;
    }
    assert(data_.size() == required_size && "Data size does not match shape dimensions");
}

// Private helper
size_t DynamicTensor::calculate_index(const std::vector<size_t>& indices) const {
    assert(indices.size() == shape_.size() && "Dimensions of index do not match tensor rank");

    size_t flat_index = 0;
    size_t stride = 1;

    // We iterate backwards (from last dimension to first) to calculate the stride.
    // Example: For shape [2, 3], index [1, 2] -> 1*3 + 2
    for (int i = static_cast<int>(shape_.size()) - 1; i >= 0; --i) {
        assert(indices[i] < shape_[i] && "Index out of bounds");
        flat_index += indices[i] * stride;
        stride *= shape_[i];
    }
    return flat_index;
}

double& DynamicTensor::at(const std::vector<size_t>& indices) {
     return data_[calculate_index(indices)];
}

const double& DynamicTensor::at(const std::vector<size_t>& indices) const {
    return data_[calculate_index(indices)];
}

double& DynamicTensor::operator[](size_t i) {
    assert(i < data_.size()); 
    return data_[i];
}

const double& DynamicTensor::operator[](size_t i) const {
    assert(i < data_.size());
    return data_[i];
}

size_t DynamicTensor::size() const {
    return data_.size();
}

const std::vector<size_t>& DynamicTensor::get_shape() const {
    return shape_;
}

size_t DynamicTensor::rank() const {
    return shape_.size();
}