#ifndef OUTPUT_FILE_H_
#define OUTPUT_FILE_H_

#include <vector>
#include <string>
#include <fstream>
#include "dynamic_tensor.h"
#include "ode_solver.h"
#include "output.h"

class OdeSolver; // Forward declaration

/**
 * @brief Abstract base class for file-based output of ODE solver results.
 * Derived classes should implement the FileSpecificSetup method to handle
 * specific file format setups.
 */
class OutputFile: public Output {
public:
    /**
     * @brief Constructor.
     * @param filename The name of the output file.
     * @param separator The separator character used in the output file.
     * @param element_separator The separator character used between elements in tensor output.
     */
    OutputFile(const std::string& filename, char separator = ';', char element_separator = ',');

    /**
     * @brief Virtual destructor.
     */
    virtual ~OutputFile() = default;

    /**
     * @brief Method to write the ODE solver results to the output file.
     * @param solver A constant reference to the OdeSolver instance containing results to output.
     */
    virtual void Write(const OdeSolver& solver) override;

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
     * @brief The separator character used between elements in tensor output (e.g., in nested structures).
     */
    char element_separator_;

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
     * Note: I did use ChatGPT to verify what I do is correct, and i used it for debugging when the tests were failing.
     */
    void WriteTensorRecursive(std::ofstream& file, const DynamicTensor& tensor, 
                                const std::vector<size_t>& shape, std::vector<size_t>& index, size_t dim) const;

    /**
     * @brief Virtual method to be overridden by derived classes for file specific setup.
     * @param file The output file stream.
     */
    virtual void FileSpecificSetup(std::ofstream& file) const = 0;

};
#endif // OUTPUT_FILE_H_