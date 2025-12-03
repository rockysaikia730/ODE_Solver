#include <string>
#include <sstream>
#include <vector>
#include "reader.h"
#include "ode_raw_data.h"
#include "dynamic_tensor.h"
#include "function.h"

Reader::Reader(const std::string& file_name, char separator)
    : file_name_(file_name), separator_(separator) {
    // Extract file extension from file name
    size_t dot_position = file_name.find_last_of('.');
    if (dot_position != std::string::npos) {
        file_extension_ = file_name.substr(dot_position + 1);
    } else {
        file_extension_ = "";
    }
}

std::string Reader::GetFileName() const {
    return file_name_;
}

std::string Reader::GetFileExtension() const {
    return file_extension_;
}

void Reader::SetFileName(const std::string& file_name) {
    file_name_ = file_name;
}

void Reader::SetFileExtension(const std::string& file_extension) {
    file_extension_ = file_extension;
}

bool Reader::IsNumeric(const std::string& str) const {
    if (str.empty()) return false;

    char* endptr = nullptr;
    std::strtod(str.c_str(), &endptr); //convert string to double, if possible
    return (*endptr == '\0'); // if endptr points to the end of the string, it's all numeric
}


bool Reader::Open() {
    file_stream_.open(file_name_);
    return file_stream_.is_open();
}

void Reader::Close() {
    if (file_stream_.is_open()) {
        file_stream_.close();
    }
}

void Reader::SetSeparator(char separator) {
    separator_ = separator;
}

char Reader::GetSeparator() const {
    return separator_;
}

std::vector<std::string> Reader::Split(const std::string& str) {
    std::vector<std::string> tokens; // to store the split tokens
    std::string token;
    std::istringstream token_stream(str); // Reading from the input string
    while (std::getline(token_stream, token, separator_)) {
        tokens.push_back(token); // add each token to the vector
    }
    return tokens;
}

std::vector<std::string> Reader::Trim(const std::vector<std::string>& tokens) {
    std::vector<std::string> trimmed_tokens; // to store the trimmed tokens

    for (const auto& token : tokens) {
        size_t start = 0; // index of the first non-space character
        size_t end = token.length(); // index one past the last non-space character

        // First non-space character from the beginning
        while (start < end && std::isspace(token[start])) {
            ++start;
        }

        // Find the last non-space character
        while (end > start && std::isspace(token[end - 1])) {
            --end;
        }

        // Extract the trimmed token
        trimmed_tokens.push_back(token.substr(start, end - start));
    }
    return trimmed_tokens;
}

DynamicTensor Reader::ParseTensor(const std::string& str) {
    std::vector<size_t> shape;
    // This will take a string representation of a tensor and parse it into a DynamicTensor object.
    // ex: [[[[1, 2], [3,4]], [[5,6],[7,8]]]] for a rank-4 tensor of shape (1,2,2,2)
    // Missing implementation
    return DynamicTensor(); // Placeholder implementation
}

void Reader::ParseFunction(const std::string& func_str) {
    // Placeholder implementation
    //strict formatting: we can read a tensor [[exp1, exp2, ...], [exp3, exp4, ...], ...]
    // The expressions must have as variables 't' and 'y', where y is a tensor. different element of the y tensor
    // must be written in the form y[0,1,...] to access the respective element. 
    // Must match the rank and shape of y0 in raw_data_

    // This Parse function will create a function object, overload its eval method to evaluate the expressions, and store
    // the shared pointer to the function in raw_data_.func
    
    // Missing implementation
}

bool Reader::IsComplexNumber(const std::string& str) const {
    // A simple check for the presence of 'i' or 'j' to indicate a complex number
    // complex numbers must be represented as (a,b) where a is the real part and b is the imaginary part
    
}

