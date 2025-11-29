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
#include <cstddef> // for size_t

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
    /**
     * @brief Default constructor.
     * Creates an empty tensor with rank 0 and size 0.
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

    /**
     * @brief Data constructor.
     * Creates a tensor with specific shape and pre-existing data.
     *
     * @param input_shape A vector containing the size of each dimension.
     * @param data The flat vector of data.
     * @warning The size of @p data must exactly match the product of @p input_shape. Otherwise it is flattened
     */
    DynamicTensor(const std::vector<double>& data, const std::vector<size_t>& input_shape = {});

    /**
     * @brief Access element at N-dimensional coordinate (Read/Write).
     * Performs bounds checking via assert().
     *
     * @param indices A vector of coordinates (e.g., {x, y, z}).
     * @return Reference to the double value at that position.
     */
    double& at(const std::vector<size_t>& indices);

    /**
     * @brief Access element at N-dimensional coordinate (Read-only).
     * Performs bounds checking via assert().
     *
     * @param indices A vector of coordinates (e.g., {x, y, z}).
     * @return Const reference to the double value.
     */
    const double& at(const std::vector<size_t>& indices) const;

    /**
     * @brief Access element by flat index (Read/Write).
     * Useful for iterating linearly through the tensor without caring about shape.
     *
     * @param i The linear index in the underlying storage.
     * @return Reference to the double value.
     */
    double& operator[](size_t i);

    /**
     * @brief Access element by flat index (Read-only).
     *
     * @param i The linear index in the underlying storage.
     * @return Const reference to the double value.
     */
    const double& operator[](size_t i) const;

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
    std::vector<double> data_;

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