#include <string>
#include <sstream>
#include <vector>
#include <complex>
#include "reader.h"
#include "ode_raw_data.h"
#include "dynamic_tensor.h"

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

std::vector<std::string> Reader::Split(const std::string& str, const char& separator) {
    std::vector<std::string> tokens; // to store the split tokens
    std::string token;
    std::istringstream token_stream(str); // Reading from the input string
    while (std::getline(token_stream, token, separator)) {
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
    std::string content;
    content.reserve(str.size());

    // Remove all whitespace from the string
    for (char c : str) {
        if (!std::isspace(c)) {
            content += c;
        }
    }


    if (content.empty()) {
        // Handle error! MISSING IMPLEMENTATION
    }


    if (!(content.front() == '[' && content.back() == ']')) {
        //Base case: single value
        
        if(IsComplexNumber(content)){
            // must be a single complex value
            return DynamicTensor(ParseComplexNumber(content));
        }
        if (IsNumeric(content)) {
            // must be a single double value
            return DynamicTensor(ParseDouble(content));
        }
        // Handle error! MISSING IMPLEMENTATION
    }

    //General case: we have a nested bracket list
    size_t pos = 0;
    bool contains_complex = HasComplexNumber(content);

    if (!contains_complex) {
        std::vector<double> data;
        std::vector<size_t> shape = ParseTensorRecursive(content, pos, data);
        return DynamicTensor(data, shape);
    } else {
        std::vector<std::complex<double>> data;
        std::vector<size_t> shape = ParseTensorRecursive(content, pos, data);
        return DynamicTensor(data, shape);
    }

}

std::vector<size_t> Reader::ParseTensorRecursive(const std::string& str, size_t& pos, std::vector<double>& data) {
    if (str[pos] != '[') {
        // Handle error! MISSING IMPLEMENTATION
    }
    ++pos; // Move past the opening bracket '['
    
    std::vector<std::vector<size_t>> lower_shapes;
    size_t current_dimension_size = data.size();
    bool detected_double = false;
    bool detected_nested = false;

    while (pos < str.size()){
        char c = str[pos];

        if (c == '['){
            detected_nested = true;
            if (detected_double) {
                // Mixed types detected
                // Handle error! MISSING IMPLEMENTATION
            }
            lower_shapes.push_back(ParseTensorRecursive(str, pos, data));
        }
        else if (c == ']'){
            ++pos;
            break; // End of current tensor level
        }
        else if (c == ','){
            ++pos; // Move past the comma
        }
        else {
            detected_double = true;
            if (detected_nested) {
                // Mixed types detected
                // Handle error! MISSING IMPLEMENTATION
            }
            // Parse a number
            size_t start_pos = pos;
            while (pos < str.size() && (std::isdigit(str[pos]) || str[pos] == '+' || str[pos] == '.' || str[pos] == '-' || str[pos] == 'e' || str[pos] == 'E')) {
                ++pos;
            }
            double value = ParseDouble(str.substr(start_pos, pos - start_pos));
            data.push_back(value); 
        }
    }

    // Determine the shape of the current tensor level
    if (detected_nested){
        const auto& first_shape = lower_shapes.front();
        for (const auto& shape : lower_shapes) {
            if (shape != first_shape) {
                // Inconsistent shapes detected
                // Handle error! MISSING IMPLEMENTATION
            }
        }

        std::vector<size_t> current_shape;
        current_shape.push_back(lower_shapes.size());
        current_shape.insert(current_shape.end(), first_shape.begin(), first_shape.end());
        return current_shape;
    }
    return {data.size() - current_dimension_size};
}

std::vector<size_t> Reader::ParseTensorRecursive(const std::string& str, size_t& pos, std::vector<std::complex<double>>& data) {
    if (str[pos] != '[') {
        // Handle error! MISSING IMPLEMENTATION
    }
    ++pos; // Move past the opening bracket '['
    
    std::vector<std::vector<size_t>> lower_shapes;
    size_t current_dimension_size = data.size();
    bool detected_complex = false;
    bool detected_nested = false;

    while (pos < str.size()){
        char c = str[pos];

        if (c == '['){
            detected_nested = true;
            if (detected_complex) {
                // Mixed types detected
                // Handle error! MISSING IMPLEMENTATION
            }
            lower_shapes.push_back(ParseTensorRecursive(str, pos, data));
        }
        else if (c == ']'){
            ++pos;
            break; // End of current tensor level
        }
        else if (c == ','){
            ++pos; // Move past the comma
        }
        else {
            detected_complex = true;
            if (detected_nested) {
                // Mixed types detected
                // Handle error! MISSING IMPLEMENTATION
            }
            // Parse a complex number
            size_t start_pos = pos;
            if (str[pos] != '(') {
                // Handle error! MISSING IMPLEMENTATION
            }
            while (pos < str.size() && str[pos] != ')') {
                ++pos;
            }

            if (pos == str.size()) {
                // Handle error! MISSING IMPLEMENTATION
            }
            ++pos; // Move past the closing parenthesis ')'
            std::complex<double> value = ParseComplexNumber(str.substr(start_pos, pos - start_pos));
            data.push_back(value); 
        }
    }

    // Determine the shape of the current tensor level
    if (detected_nested){
        const auto& first_shape = lower_shapes.front();
        for (const auto& shape : lower_shapes) {
            if (shape != first_shape) {
                // Inconsistent shapes detected
                // Handle error! MISSING IMPLEMENTATION
            }
        }

        std::vector<size_t> current_shape;
        current_shape.push_back(lower_shapes.size());
        current_shape.insert(current_shape.end(), first_shape.begin(), first_shape.end());
        return current_shape;
    }
    return {data.size() - current_dimension_size};
}

bool Reader::IsComplexNumber(const std::string& str) const {
    // A simple check for the presence of 'i' or 'j' to indicate a complex number
    // complex numbers must be represented as (a,b) where a is the real part and b is the imaginary part
    if (str.size() < 5) return false; // minimum size for a complex number representation "(a,b)"
    if (str.front() != '(' || str.back() != ')') return false;

    std::string content = str.substr(1, str.size() - 2); // remove parantheses

    //split by comma and trim spaces
    auto parts = Split(content, ',');
    parts = Trim(parts);

    if (parts.size() != 2) return false;

    return IsNumeric(parts[0]) && IsNumeric(parts[1]);
}


std::string Reader::ToLower(const std::string& str) const {
    std::string lower_str = str;
    for (char& c : lower_str) {
        c = static_cast<char>(std::tolower(static_cast<char>(c)));
    }
    return lower_str;
}

bool Reader::LineStartsWith(const std::string& line, const std::string& start) const {
    if (line.size() < start.size()) {
        return false;
    }
    return line.substr(0, start.size()) == start;
}

bool Reader::LineEndsWith(const std::string& line, const std::string& end) const {
    if (line.size() < end.size()) {
        return false;
    }
    return line.substr(line.size() - end.size(), end.size()) == end;
}

std::complex<double> Reader::ParseComplexNumber(const std::string& str) const {
    // Assumes str is in the format "(a,b)" where a is the real part and b is the imaginary part
    if (!IsComplexNumber(str)) {
        // Handle error! MISSING IMPLEMENTATION
    }

    std::string content = str.substr(1, str.size() - 2); // remove parentheses
    auto parts = Split(content, ',');
    parts = Trim(parts);

    double real = std::stod(parts[0]);
    double imag = std::stod(parts[1]);

    return std::complex<double>(real, imag);
}

double Reader::ParseDouble(const std::string& str) const {
    try {
        return std::stod(str);
    } catch (const std::invalid_argument& e) {
        // Handle error! MISSING IMPLEMENTATION
    } catch (const std::out_of_range& e) {
        // Handle error! MISSING IMPLEMENTATION
    }
    return 0.0; // Default return value in case of error
}

bool Reader::HasComplexNumber(const std::string& str) const {
    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == '(') {
            // Look for the closing parenthesis
            size_t j = str.find(')', i);

            if (j != std::string::npos) {

                //Make substring and check if it's a complex number
                std::string potential_complex = str.substr(i, j - i + 1);
                if (IsComplexNumber(potential_complex)) {
                    // check if it's a valid complex number
                    return true;
                }
                i = j; // Move index to the closing parenthesis
            }
        }
    }
    return false;
}



