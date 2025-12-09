#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <vector>
#include <string>
#include <fstream>
#include "dynamic_tensor.h"
#include "ode_solver.h"

class OdeSolver; // Forward declaration

/**
 * @brief Abstract base class for outputting ODE solver results.
 * Derived classes should implement the Write method to handle specific output formats.
 */
class Output {
public:
    /**
     * @brief Constructor.
     * 
     * @param filename The name of the output file.
     */
    Output(const std::string& filename, char separator = ',');

    /**
     * @brief Virtual destructor.
     */
    virtual ~Output() = default;

    /**
     * @brief Pure virtual method to write the results of the ODE solver.
     * @param solver A constant reference to the OdeSolver instance containing results to output.
     */
    virtual void Write(const OdeSolver& solver) = 0;

    /**
     * @brief Gets the output filename.
     */
    std::string GetFilename() const;

    /**
     * @brief Sets the output filename.
     * @param filename The new output filename.
     */
    void SetFilename(const std::string& filename);

protected:
    /**
     * @brief The name of the output file.
     */
    std::string filename_;

    /**
     * @brief The separator character used in the output file.
     */
    char separator_;

    /** 
     * @brief Ensures the output file exists; creates it if it does not.
     */
    void EnsureFileExists() const;

    /**
     * @brief A helper to write recursive data structures if needed.
     * @param file The output file stream.
     * @param tensor The DynamicTensor to write. Can be complex or real. Complex is handled as (real, imag).
     * @param shape The shape of the tensor.
     * @param depth The current recursion depth.
     * @param separator The separator character for formatting.
     */
    void WriteTensorRecursive(std::ofstream& file, const DynamicTensor& tensor, 
                                const std::vector<size_t>& shape, std::vector<size_t>& index, size_t dim, char separator) const;


};
#endif // OUTPUT_H_