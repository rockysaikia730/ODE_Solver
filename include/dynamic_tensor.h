/**
 * @file dynamic_tensor.h
 * @brief Header definition for the DynamicTensor class.
 *
 * This file defines a container that allows for runtime definition of
 * tensor dimensions (rank and shape) using a flattened memory model.
 */

#ifndef DYNAMIC_TENSOR_H_
#define DYNAMIC_TENSOR_H_

#include <vector>
#include <cstddef> 
#include <complex>
#include <variant>
#include <iostream>
/**
 * @class DynamicTensor
 * @brief A class to represent a mathematical tensor of arbitrary rank at runtime.
 *
 * The DynamicTensor stores data in a contiguous 1D memory block (std::vector)
 * but provides an interface to access elements using N-dimensional coordinates.
 * It uses Row-Major ordering (last dimension changes fastest).
 *
 * @example
 * std::vector<size_t> shape = {2, 3}; // 2x3 Matrix
 * DynamicTensor t(shape, 1.0);
 * t.at({0, 1}) = 5.0;
 */
class DynamicTensor {
public:
    enum class Type { REAL, COMPLEX };
    using Complex = std::complex<double>;
    /**
     * @brief Default constructor.
     * Creates an empty real tensor with rank 0 and size 0.
     */
    DynamicTensor();

    /**
     * @brief Shape constructor.
     * Allocates memory based on the product of the shape dimensions.
     *
     * @param input_shape A vector containing the size of each dimension.
     * @param initial_value The value to initialize all elements with.
     */
    DynamicTensor(const std::vector<size_t>& input_shape, double initial_value);
    DynamicTensor(const std::vector<size_t>& input_shape, Complex initial_value);
    /**
     * @brief Data constructor.
     * Creates a tensor with specific shape and pre-existing data.
     *
     * @param input_shape A vector containing the size of each dimension.
     * @param data The flat vector of data.
     * @warning The size of @p data must exactly match the product of @p input_shape. Otherwise it is flattened
     */
    DynamicTensor(const std::vector<double>& data, const std::vector<size_t>& input_shape = {});
    DynamicTensor(const std::vector<Complex>& data, const std::vector<size_t>& input_shape = {});

    DynamicTensor(double data);
    DynamicTensor(Complex data);

    bool IsComplex() const;
    Type GetType() const;

    /**
     * @brief Unified Templated Accessor.
     * Usage: tensor.at<double>({0}) or tensor.at<Complex>({0})
     */
    template <typename T>
    T& at(const std::vector<size_t>& indices) {return GetVec<T>()[calculate_index(indices)];}

    template <typename T>
    const T& at(const std::vector<size_t>& indices) const {return GetVec<T>()[calculate_index(indices)];}

    /**
     * @brief Universal Templated Iterators
     * Usage: auto it = tensor.begin<double>();
     */
    template <typename T>
    auto begin() {return GetVec<T>().begin();}

    template <typename T>
    auto end() {return GetVec<T>().end();}

    template <typename T>
    auto begin() const {return GetVec<T>().begin();}

    template <typename T>
    auto end() const {return GetVec<T>().end();}

    /**
     * @brief Element-wise Addition.
     * Handles Real+Real and Complex+Complex
     */
    DynamicTensor operator+(const DynamicTensor& second_tensor) const;

    /**
     * @brief Broadcasting with Real scalar 
     */
    DynamicTensor operator+(double scalar) const;
    friend DynamicTensor operator+(double scalar, const DynamicTensor& tensor);

    /**
     * @brief Broadcasting with Complex scalar 
     */
    DynamicTensor operator+(Complex scalar) const;
    friend DynamicTensor operator+(Complex scalar, const DynamicTensor& tensor);

    /**
     * @brief Scalar Multiplication (Tensor * double).
     */
    DynamicTensor operator*(double scalar) const;
    friend DynamicTensor operator*(double scalar, const DynamicTensor& tensor);

    /**
     * @brief Element-wise Subtraction (t1 - t2)
     */
    DynamicTensor operator-(const DynamicTensor& second_tensor) const;

    /**
     * @brief Element-wise Multiplication (t1 - t2)
     */
    DynamicTensor operator*(const DynamicTensor& second_tensor) const;

    /**
     * @brief Element-wise Division (t1 / t2)
     */
    DynamicTensor operator/(const DynamicTensor& second_tensor) const;
    /**
     * @brief Scalar Division (Tensor / double).
     */
    DynamicTensor operator/(double scalar) const;

    /**
     * @brief Get the total number of elements in the tensor.
     * @return The product of all dimensions (e.g., 2x3x4 = 24).
     */
    size_t size() const;

    /**
     * @brief Get the shape of the tensor.
     * @return A const reference to the vector of dimensions.
     */
    const std::vector<size_t>& get_shape() const;

    /**
     * @brief Get the rank (number of dimensions) of the tensor.
     * @return The size of the shape vector (e.g., a 3D volume returns 3).
     */
    size_t rank() const;

    /**
     * @brief Get the flattened data as a vector.
     * @return A constant reference to the internal data vector.
     * @note The changes here are suggested by ChatGPT.
     */
    template <typename T>
    const T& flat(size_t index) const {
        return GetVec<T>()[index];
    }

    /**
     * @brief Get the flattened data as a vector.
     * @return A reference to the internal data vector. The caller can modify the data.
     * @note The changes here are suggested by ChatGPT.
     */
    template <typename T>
    T& flat(size_t index) {
        return GetVec<T>()[index];
    }


private:
    /// @brief Flattened storage of tensor data.
    std::variant<std::vector<double>, std::vector<Complex>> data_;

    /// @brief Dimensions of the tensor.
    std::vector<size_t> shape_;

    /**
     * @brief Helper to convert N-dimensional indices to a 1D flat index.
     * Uses Row-Major ordering logic.
     *
     * @param indices The N-dimensional coordinates.
     * @return The calculated linear index.
     */
    size_t calculate_index(const std::vector<size_t>& indices) const;


    /**
     * @brief Helper to return reference to the specific std::vector
     *
     * @return Rference to the specific std::vector
     */
    template <typename T>
    std::vector<T>& GetVec() {
        if constexpr (std::is_same_v<T, double>) {
            if (IsComplex()) throw std::runtime_error("Type Mismatch: Tensor is Complex, requested double.");
            return std::get<std::vector<double>>(data_);
        } 
        else if constexpr (std::is_same_v<T, Complex>) {
            if (!IsComplex()) throw std::runtime_error("Type Mismatch: Tensor is Real, requested Complex.");
            return std::get<std::vector<Complex>>(data_);
        } 
    }

    /**
     * @brief Const helper to return reference to the specific std::vector
     *
     * @return Rference to the specific std::vector
     */
    template <typename T>
    const std::vector<T>& GetVec() const {
        if constexpr (std::is_same_v<T, double>) {
            if (IsComplex()) throw std::runtime_error("Type Mismatch: Tensor is Complex, requested double.");
            return std::get<std::vector<double>>(data_);
        } 
        else if constexpr (std::is_same_v<T, Complex>) {
            if (!IsComplex()) throw std::runtime_error("Type Mismatch: Tensor is Real, requested Complex.");
            return std::get<std::vector<Complex>>(data_);
        } 
    }
};

inline std::ostream& operator<<(std::ostream& os, const DynamicTensor& tensor) {
    os << "DynamicTensor(shape=[";

    const auto& shape = tensor.get_shape();
    for (size_t i = 0; i < shape.size(); ++i) {
        os << shape[i];
        if (i < shape.size() - 1) {
            os << ", ";
        }
    }

    //printing in flat form.
    os << "], data=[";
    size_t total_size = tensor.size();
    for (size_t i = 0; i < total_size; ++i) {
        if (tensor.IsComplex()) {
            os << tensor.flat<DynamicTensor::Complex>(i);
        } else {
            os << tensor.flat<double>(i);
        }
        if (i < total_size - 1) {
            os << ", ";
        }
    }
    os << "])";
    return os;
}

#endif