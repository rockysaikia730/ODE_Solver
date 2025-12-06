#include "dynamic_tensor.h"

#include <vector>
#include <cassert>

// Default constructor
DynamicTensor::DynamicTensor() : data_(std::vector<double>()) {}

// Constructor for scalars
DynamicTensor::DynamicTensor(double data)
    : data_(std::vector<double>{data}),
      shape_({1}) {}

DynamicTensor::DynamicTensor(Complex data)
    : data_(std::vector<Complex>{data}),
      shape_({1}) {}

// Constructor with shape for Real
DynamicTensor::DynamicTensor(const std::vector<size_t>& input_shape, double initial_value)
    : shape_(input_shape) {
    size_t total_size = 1;
    if (shape_.empty()) {
        total_size = 0;
    } else {
        for (size_t dim : shape_) total_size *= dim;
    }
    data_ = std::vector<double>(total_size, initial_value);
}
// Constructor with shape for Complex
DynamicTensor::DynamicTensor(const std::vector<size_t>& input_shape, Complex initial_value)
    : shape_(input_shape) {
    size_t total_size = 1;
    if (shape_.empty()) {
        total_size = 0;
    } else {
        for (size_t dim : shape_) total_size *= dim;
    }
    data_ = std::vector<Complex>(total_size, initial_value);
}

// Constructor with shape and data for Real
DynamicTensor::DynamicTensor(const std::vector<double>& data, const std::vector<size_t>& input_shape)
    : data_(data) {
    if (input_shape.empty()) {
        if (std::get<std::vector<double>>(data_).empty()) {
            shape_ = {};
        } else {
            shape_ = {std::get<std::vector<double>>(data_).size()};
        }
    } else {
        shape_ = input_shape;
        size_t required_size = 1;
        for (size_t dim : shape_) required_size *= dim;
        assert(std::get<std::vector<double>>(data_).size() == required_size && "Data size mismatch");
    }
}
// Constructor with shape and data for Complex
DynamicTensor::DynamicTensor(const std::vector<Complex>& data, const std::vector<size_t>& input_shape)
    : data_(data) {
    
    // Logic to infer shape if missing
    if (input_shape.empty()) {
        if (std::get<std::vector<Complex>>(data_).empty()) {
            shape_ = {};
        } else {
            shape_ = {std::get<std::vector<Complex>>(data_).size()};
        }
    } else {
        shape_ = input_shape;
        size_t required_size = 1;
        for (size_t dim : shape_) required_size *= dim;
        assert(std::get<std::vector<Complex>>(data_).size() == required_size && "Data size mismatch");
    }
}

// Checks if data is Complex or Real
bool DynamicTensor::IsComplex() const {
    return std::holds_alternative<std::vector<Complex>>(data_);
}
DynamicTensor::Type DynamicTensor::GetType() const {
    return IsComplex() ? Type::COMPLEX : Type::REAL;
}

// Private helper
size_t DynamicTensor::calculate_index(const std::vector<size_t>& indices) const {
    assert(indices.size() == shape_.size() && "Dimensions of index do not match tensor rank");

    size_t flat_index = 0;
    size_t stride = 1;

    // We iterate backwards (from last dimension to first) to calculate the stride.
    // Example: For shape [2, 3], index [1, 2] -> 1*3 + 2
    for (int i = static_cast<int>(shape_.size()) - 1; i >= 0; i--) {
        assert(indices[i] < shape_[i] && "Index out of bounds");
        flat_index += indices[i] * stride;
        stride *= shape_[i];
    }
    return flat_index;
}

// Adding two tensors (t1 + t2)
DynamicTensor DynamicTensor::operator+(const DynamicTensor& second_tensor) const {
    assert(shape_ == second_tensor.shape_ && "Shape mismatch in addition");
    if (GetType() != second_tensor.GetType()) throw std::runtime_error("Strict Mode: Cannot add Real and Complex tensors directly.");
    
    DynamicTensor result;
    result.shape_ = shape_;
    if (IsComplex()) {
        const auto& v1 = std::get<std::vector<Complex>>(data_);
        const auto& v2 = std::get<std::vector<Complex>>(second_tensor.data_);
        auto& res_vec = result.data_.emplace<std::vector<Complex>>(v1.size());
        for (size_t i = 0; i < v1.size(); i++) res_vec[i] = v1[i] + v2[i];
    } else {
        const auto& v1 = std::get<std::vector<double>>(data_);
        const auto& v2 = std::get<std::vector<double>>(second_tensor.data_);
        auto& res_vec = result.data_.emplace<std::vector<double>>(v1.size());
        for (size_t i = 0; i < v1.size(); i++) res_vec[i] = v1[i] + v2[i];
    }

    return result;
}

// Multiplication with real scalars (t1 * h)
DynamicTensor DynamicTensor::operator*(double scalar) const {
    DynamicTensor result;
    result.shape_ = shape_;
    if (IsComplex()) {
        const auto& vec = std::get<std::vector<Complex>>(data_);
        auto& res_vec = result.data_.emplace<std::vector<Complex>>(vec.size());
        for (size_t i = 0; i < vec.size(); ++i) res_vec[i] = vec[i] * scalar;
    } else {
        const auto& vec = std::get<std::vector<double>>(data_);
        auto& res_vec = result.data_.emplace<std::vector<double>>(vec.size());
        for (size_t i = 0; i < vec.size(); ++i) res_vec[i] = vec[i] * scalar;
    }
    return result;
}

DynamicTensor operator*(double scalar, const DynamicTensor& tensor) {
    return tensor * scalar;
}

DynamicTensor DynamicTensor::operator-(const DynamicTensor& second_tensor) const {
    return *this + (-1.0 * second_tensor);
}

DynamicTensor DynamicTensor::operator/(double scalar) const {
    if (scalar == 0.0) throw std::runtime_error("Division by zero");
    return *this * (1/scalar);
}

size_t DynamicTensor::size() const {
    if (IsComplex()) {
        return std::get<std::vector<Complex>>(data_).size();
    }
    return std::get<std::vector<double>>(data_).size();
}

const std::vector<size_t>& DynamicTensor::get_shape() const {
    return shape_;
}

size_t DynamicTensor::rank() const {
    return shape_.size();
}