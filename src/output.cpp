#include <string>
#include <fstream>
#include "output.h"

Output::Output(const std::string& filename, char separator)
    : filename_(filename), separator_(separator) {
    EnsureFileExists();
    }

std::string Output::GetFilename() const {
    return filename_;
}

void Output::EnsureFileExists() const {
    std::ofstream file(filename_, std::ios::app);
    // The file is created if it does not exist; no need to write anything.
    file.close();
}

void Output::SetFilename(const std::string& filename) {
    filename_ = filename;
    EnsureFileExists();
}

void Output::WriteTensorRecursive(std::ofstream& file, const DynamicTensor& tensor, 
                                  const std::vector<size_t>& shape, std::vector<size_t>& index, size_t dim, char separator) const {
    if (dim == shape.size()) {
        // Write the element of the tensor
        if(tensor.IsComplex()) {
            auto value = tensor.at<DynamicTensor::Complex>(index);
            file << "(" << value.real() << ", " << value.imag() << ")";
        } else {
            file << tensor.at<double>(index);
        }
    }
    else {
        file << "[";
        for (size_t i = 0; i < shape[dim]; ++i) {
            index[dim] = i;
            WriteTensorRecursive(file, tensor, shape, index, dim + 1, separator);
            if (i + 1 < shape[dim]) {
                file << separator << " ";
            }
        }
        file << "]";
    }
}