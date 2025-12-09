#include <string>
#include <sstream>
#include <vector>
#include <complex>
#include <iostream>
#include <memory>
#include "reader.h"
#include "ode_raw_data.h"
#include "dynamic_tensor.h"
#include "parsed_function.h"

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

std::vector<std::string> Reader::Split(const std::string& str, const char& separator) const {
    std::vector<std::string> tokens; // to store the split tokens
    std::string token;
    std::istringstream token_stream(str); // Reading from the input string
    while (std::getline(token_stream, token, separator)) {
        tokens.push_back(token); // add each token to the vector
    }
    return tokens;
}

std::vector<std::string> Reader::Trim(const std::vector<std::string>& tokens) const {
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
    size_t pos = 0;
    if (HasComplexNumber(str)) {
        std::vector<DynamicTensor::Complex> data;
        std::vector<size_t> shape = ParseTensorRecursive(str, pos, data);
        return DynamicTensor(data, shape);
    } else {
        std::vector<double> data;
        std::vector<size_t> shape = ParseTensorRecursive(str, pos, data);
        return DynamicTensor(data, shape);
    }
}

template <typename T>
std::vector<size_t> Reader::ParseTensorRecursive(const std::string& str, size_t& pos, std::vector<T>& data)
{
    std::vector<size_t> shape;
    size_t element_count = 0; // number of elements in current dimension

    //assume first char is '['
    ++pos;

    while (pos < str.size()) {

        if (str[pos] == '[') {
            // Start of a new dimension
            std::vector<T> nested_data;
            std::vector<size_t> nested_shape =
                ParseTensorRecursive(str, pos, nested_data);

            data.insert(data.end(), nested_data.begin(), nested_data.end());

            if (element_count == 0) {
                shape = nested_shape;
            }
            else if (shape != nested_shape) {
                // Handle error! MISSING IMPLEMENTATION
                throw std::invalid_argument("Inconsistent tensor shape detected.");
            }
            ++element_count;
        }

        else if (str[pos] == ']') {
            // End of this dimension
            ++pos;
            break;
        }

        else if (str[pos] == ',' || std::isspace(static_cast<unsigned char>(str[pos]))) {
            // Skip separators and whitespace
            ++pos;
        }

        else {
            // Parse a scalar or complex number
            std::string value_str;

            if (str[pos] == '(') {
                // Complex number: read matched parentheses
                int depth = 0;
                do {
                    if (str[pos] == '(') depth++;
                    else if (str[pos] == ')') depth--;

                    value_str += str[pos];
                    ++pos;
                } while (pos < str.size() && depth > 0);
            }
            else {
                // Real number: read until next separator
                while (pos < str.size() &&
                       str[pos] != ',' &&
                       str[pos] != ']' &&
                       str[pos] != '[' &&
                       !std::isspace(static_cast<unsigned char>(str[pos])))
                {
                    value_str += str[pos];
                    ++pos;
                }
            }

            value_str = TrimString(value_str);

            // Convert the string into a value of type T
            T value;

            if constexpr (std::is_same_v<T, double>) {
                value = ParseDouble(value_str);
            }
            else if constexpr (std::is_same_v<T, DynamicTensor::Complex>) {
                if (!value_str.empty() &&
                    value_str.front() == '(' &&
                    value_str.back() == ')')
                {
                    value = ParseComplexNumber(value_str);
                }
                else {
                    double real_part = ParseDouble(value_str);
                    value = DynamicTensor::Complex(real_part, 0.0);
                }
            }

            data.push_back(value);
            ++element_count;
        }
    }

    // Compute the actual shape
    if (shape.empty()) {
        return { data.size() };
    }
    else {
        shape.insert(shape.begin(), element_count);
        return shape;
    }
}

bool Reader::IsComplexNumber(const std::string& str) const {
    // A simple check for the strict format of complex numbers
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
        throw std::invalid_argument("Invalid complex number format: " + str);
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
        //HANDLE ERROR! MISSING IMPLEMENTATION
        throw std::invalid_argument("Invalid double format: " + str);
    } catch (const std::out_of_range& e) {
        //HANDLE ERROR! MISSING IMPLEMENTATION
        throw std::out_of_range("Double value out of range: " + str);
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

void Reader::ParseSolverMethodFromString(const std::string& method_string) {
        if (method_string == "ExplicitEuler") {
            raw_data_.solver_params.method = SolverMethod::kExplicitEuler;
        } else if (method_string == "RungeKutta4") {
            raw_data_.solver_params.method = SolverMethod::kRungeKutta4;
        } else if (method_string == "ImplicitEuler") {
            raw_data_.solver_params.method = SolverMethod::kImplicitEuler;
        } else if (method_string == "AdamsBashforth") {
            raw_data_.solver_params.method = SolverMethod::kAdamsBashforth;
        } else {
            // Default or unknown method handling
            raw_data_.solver_params.method = SolverMethod::kUnknown;
        }
    }


void Reader::InterpretKeyValuePair(const std::string& key, const std::string& value) {
    bool derivative_read = false;

    if (key == "t") {
        raw_data_.time_params.t0 = ParseDouble(value);
    }
    else if (key == "tf") {
        raw_data_.time_params.t_final = ParseDouble(value);
    }
    else if (key == "y") {
        raw_data_.y0 = ParseTensor(value);
    }
    else if (key == "step_size") {
        raw_data_.time_params.step_size = ParseDouble(value);
    }
    else if (key == "number_of_steps") {
        raw_data_.time_params.number_of_steps = static_cast<size_t>(ParseDouble(value));
    }
    else if (key == "solver_method") {
        //lower for this:
        std::string lower_value = ToLower(value);
        ParseSolverMethodFromString(value);
    }
    else if (key == "tolerance") {
        raw_data_.solver_params.tolerance = ParseDouble(value);
    }
    else if (key == "max_iterations") {
        raw_data_.solver_params.max_iterations = static_cast<size_t>(ParseDouble(value));
    }
    else if (key == "function") {
    raw_data_.function_params.function_expressions.clear();
    size_t pos = 0;
    raw_data_.function_params.function_shape = ParseFunctionRecursive(value, pos, raw_data_.function_params.function_expressions);
}
    else if(key == "derivative"){
        size_t pos = 0;
        raw_data_.function_params.derivative_shape = ParseFunctionRecursive(value, pos, raw_data_.function_params.derivative_expressions);
        derivative_read = true;
    }
    else{
        // skip unknown keys for now
    }
    // Does not create Function yet, just stores expressions and shape
}

std::string Reader::TrimString(const std::string& str) const {
    size_t start = 0;
    size_t end = str.size();

    while (start < end && std::isspace( static_cast<unsigned char>(str[start]))) start++;
    while (end > start && std::isspace(static_cast<unsigned char>(str[end - 1]))) end--;

    return str.substr(start, end - start);
}

std::vector<size_t> Reader::ParseFunctionRecursive(const std::string& str, size_t& pos, std::vector<std::string>& flat_expressions) {
    std::vector<size_t> sub_shape;
    size_t element_count = 0; // number of elements in current dimension

    // assume first char is '['
    ++pos;

    while(pos < str.size()) {
        if (str[pos] == '[') {
            std::vector<std::string> nested_expression;
            auto nested_shape = ParseFunctionRecursive(str, pos, nested_expression); //Recursive call on nested dimension

            flat_expressions.insert(flat_expressions.end(), nested_expression.begin(), nested_expression.end()); // Flatten expressions

            if (sub_shape.empty()) {
                sub_shape = nested_shape;
            }

            else if (sub_shape != nested_shape){
                // Handle error! MISSING IMPLEMENTATION
                throw std::invalid_argument("Inconsistent function shape detected.");
            }
            ++element_count;
        }
        else if (str[pos] == ']') {
            // End of the current dimension
            ++pos; // Move past ']'
            break;
        }
        else if (str[pos] == ',' || std::isspace(str[pos])) {
            // Separator between subdimension elements, or if space, just move on
            ++pos;
        }
        else {
            std::string token;
            while (pos < str.size() && str[pos] != ',' && str[pos] != ']' && str[pos] != '[') {
                token += str[pos];
                ++pos;
            }
            token = TrimString(token);
            flat_expressions.push_back(token);
            ++element_count;
        }
    }

    // Compute the actual shape

    if (sub_shape.empty()) {
        return {flat_expressions.size()}; // First dimension size
    }
    else {
       // size of first dimension
        sub_shape.insert(sub_shape.begin(), element_count);
        return sub_shape;
    }
}

