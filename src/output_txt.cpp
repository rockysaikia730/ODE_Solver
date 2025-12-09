#include <fstream>
#include "output.h"
#include "ode_solver.h"
#include "output_txt.h"

OutputTxt::OutputTxt(const std::string& filename, char separator, char comment_char)
    : Output(filename, separator), comment_char_(comment_char) {
        EnsureFileExists();
    }

void OutputTxt::SetCommentChar(char comment_char) {
    comment_char_ = comment_char;
}

char OutputTxt::GetCommentChar() const {
    return comment_char_;
}

std::vector<std::string> OutputTxt::GetHeader() const {
    return header_;
}

void OutputTxt::SetHeader(const std::vector<std::string>& header) {
    header_ = header;
}

void OutputTxt::Write(const OdeSolver& solver) {
    std::ofstream file(filename_, std::ios::binary); // Open in binary mode to avoid newline translation
    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open the output file: " + filename_);
    }

    // Write comment lines
    for (const auto& line : header_) {
        file << comment_char_ << " " << line << "\n";
    }

    // Write metadata (human readable)
    file << "t" << separator_ << solver.GetCurrentTime() << "\n";
    file << "step_size" << separator_ << solver.GetStepSize() << "\n";
    file << "number_of_steps" << separator_ << solver.GetNumberOfSteps() << "\n\n";

    // Write tensor in nested form
    const DynamicTensor& solution = solver.GetSolution();
    std::vector<size_t> shape = solution.get_shape();
    std::vector<size_t> index(shape.size(), 0);

    file << "y" << separator_ << " ";
    WriteTensorRecursive(file, solution, shape, index, 0, ',');
    file << "\n";
}