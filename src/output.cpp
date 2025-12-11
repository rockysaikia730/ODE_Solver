#include <string>
#include <fstream>
#include "output.h"

//---------------------------------------------------------------------//
// Constructor(s)
//---------------------------------------------------------------------//

Output::Output(const std::string& filename, char separator, char element_separator)
    : filename_(filename), separator_(separator), element_separator_(element_separator) {
    EnsureFileExists();
    }

//---------------------------------------------------------------------//
// Getters and Setters
//---------------------------------------------------------------------//
void Output::SetFilename(const std::string& filename) {
    filename_ = filename;
    EnsureFileExists();
}

std::string Output::GetFilename() const {
    return filename_;
}

//---------------------------------------------------------------------//
// Protected Methods
//---------------------------------------------------------------------//

// A helper to write recursive data structures if needed. LLMs helped here.
void Output::WriteTensorRecursive(std::ofstream& file, const DynamicTensor& tensor, 
                                  const std::vector<size_t>& shape, std::vector<size_t>& index, size_t dim) const {

    // Base case: reached the last dimension
    if (dim == shape.size()) {

        // Write the element of the tensor
        if(tensor.IsComplex()) {
            auto value = tensor.at<DynamicTensor::Complex>(index);
            file << "(" << value.real() << ", " << value.imag() << ")";
        } else {
            file << tensor.at<double>(index);
        }
    }

    // Recursive case: iterate over the current dimension
    else {
        // Open bracket for the current dimension
        file << "[";
        for (size_t i = 0; i < shape[dim]; ++i) {
            index[dim] = i;
            WriteTensorRecursive(file, tensor, shape, index, dim + 1); // Recurse to the next dimension
            if (i + 1 < shape[dim]) {
                file << element_separator_ << " "; // Separator between elements
            }
        }
        // Close bracket for the current dimension
        file << "]";
    }
}

// Make sure the file exists, create if not
void Output::EnsureFileExists() const {

    // Open in binary mode to avoid newline translation. This was suggested by ChatGPT, when the tests failed due to newlines.
    std::ofstream file(filename_, std::ios::app | std::ios::binary); 
    
    // The file is created if it does not exist; no need to write anything.
    file.close();
}

//Writer method implementation
void Output::Write(const OdeSolver& solver) const {
    std::ofstream file(filename_, std::ios::binary); // Open in binary mode to avoid newline translation (done due to suggestion by ChatGPT, when tests were failing)
    
    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open the output file: " + filename_);
    }

    // Call file specific setup like writing headers or comments
    FileSpecificSetup(file);

    // Writing final time and other numerical parameters in columns
    file << "t" << separator_ << solver.GetCurrentTime() << "\n";
    file << "step_size" << separator_ << solver.GetStepSize() << "\n";
    file << "number_of_steps" << separator_ << solver.GetNumberOfSteps() << "\n";

    // Write tensor in nested form
    const DynamicTensor& solution = solver.GetSolution();
    std::vector<size_t> shape = solution.get_shape();
    std::vector<size_t> index(shape.size(), 0); // Initialize index vector with zeros

    // Write tensor label
    file << "y" << separator_ << " ";
    WriteTensorRecursive(file, solution, shape, index, 0);
    file << "\n";
}