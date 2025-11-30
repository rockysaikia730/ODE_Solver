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
    using Complex = std::complex<double>;
    enum class Type { REAL, COMPLEX };
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

    bool IsComplex() const;
    Type GetType() const;

    /**
     * @brief Unified Templated Accessor.
     * Usage: tensor.at<double>({0}) or tensor.at<Complex>({0})
     */
    template <typename T>
    T& at(const std::vector<size_t>& indices) {
        if constexpr (std::is_same_v<T, double>) {
            return std::get<std::vector<double>>(data_)[calculate_index(indices)];
        } else if constexpr (std::is_same_v<T, Complex>) {
            return std::get<std::vector<Complex>>(data_)[calculate_index(indices)];
        }
    }

    template <typename T>
    const T& at(const std::vector<size_t>& indices) const {
        if constexpr (std::is_same_v<T, double>) {
            return std::get<std::vector<double>>(data_)[calculate_index(indices)];
        } else if constexpr (std::is_same_v<T, Complex>) {
            return std::get<std::vector<Complex>>(data_)[calculate_index(indices)];
        }
    }

    /**
     * @brief Element-wise Addition.
     * Handles Real+Real, Complex+Complex
     */
    DynamicTensor operator+(const DynamicTensor& second_tensor) const;

    /**
     * @brief Scalar Multiplication (Tensor * double).
     */
    DynamicTensor operator*(double scalar) const;

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
};

#endif // DYNAMIC_TENSOR_H_