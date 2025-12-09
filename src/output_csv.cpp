#include <string>
#include <cctype>
#include <fstream>
#include "output_csv.h"
#include "ode_solver.h"

OutputCsv::OutputCsv(const std::string& filename, char separator)
    : Output(filename, separator) {
        EnsureFileExists();
    }

char OutputCsv::GetSeparator() const {
    return separator_;  
}

void OutputCsv::SetSeparator(char separator) {
    separator_ = separator;
}

std::vector<std::string> OutputCsv::GetHeader() const {
    return header_;
}

void OutputCsv::SetHeader(const std::vector<std::string>& header) {
    header_ = header;
}
void OutputCsv::Write(const OdeSolver& solver) {

    // Overwrite file, do NOT append for final-only output
    std::ofstream file(filename_, std::ios::binary); // Open in binary mode to avoid newline translation

    if(!header_.empty()) {
        for(size_t i = 0; i < header_.size(); ++i) {
            file << header_[i];
            if(i + 1 < header_.size()) {
                file << separator_;
            }
        }
        file << "\n";
    }

    // Writing final time  and other numerical parameters
    file << "t" << separator_ << solver.GetCurrentTime() << "\n";
    file << "step_size" << separator_ << solver.GetStepSize() << "\n";
    file << "number_of_steps" << separator_ << solver.GetNumberOfSteps() << "\n";

    // writing solution
    const DynamicTensor& solution = solver.GetSolution();
    std::vector<size_t> shape = solution.get_shape();
    std::vector<size_t> index(shape.size(), 0);
    file << "y" << separator_;
    WriteTensorRecursive(file, solution, shape, index, 0, ',');

    file << "\n";
    file.close();
}


