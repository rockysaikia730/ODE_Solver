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

//------------------------------------------------------------//
// Constructor(s)
//------------------------------------------------------------//
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

//------------------------------------------------------------//
// Getters
//------------------------------------------------------------//

// return the file name
std::string Reader::GetFileName() const {
    return file_name_;
}

// return the file extension
std::string Reader::GetFileExtension() const {
    return file_extension_;
}

// return the separator character
char Reader::GetSeparator() const {
    return separator_;
}


//------------------------------------------------------------//
// Setters
//------------------------------------------------------------//

// set the file name
void Reader::SetFileName(const std::string& file_name) {
    file_name_ = file_name;
}

// set the file extension
void Reader::SetFileExtension(const std::string& file_extension) {
    file_extension_ = file_extension;
}

// set the separator character
void Reader::SetSeparator(char separator) {
    separator_ = separator;
}

//------------------------------------------------------------//
// Other Public Method(s)
//------------------------------------------------------------//
bool Reader::Open() {
    file_stream_.open(file_name_);
    return file_stream_.is_open();
}

void Reader::Close() {
    if (file_stream_.is_open()) {
        file_stream_.close();
    }
}

//------------------------------------------------------------//
// Protected Method(s)
//------------------------------------------------------------//

// Splits a string into tokens based on a delimiter.
std::vector<std::string> Reader::Split(const std::string& str, const char& separator) const {

    // to store the split tokens
    std::vector<std::string> tokens; 
    std::string token;
    
    // Reading from the input string
    std::istringstream token_stream(str);
    
    // add each token to the vector
    while (std::getline(token_stream, token, separator)) {
        tokens.push_back(token); 
    }

    return tokens;
}

// Check if a string represents a numeric value
bool Reader::IsNumeric(const std::string& str) const {
    if (str.empty()) {
        return false;
    }

    
    char* endptr = nullptr;

    //convert string to double, if possible
    std::strtod(str.c_str(), &endptr); 

    // if endptr points to the end of the string, it's all numeric
    return (*endptr == '\0'); 
}

// Trims whitespace from each token in a vector of strings.
std::vector<std::string> Reader::Trim(const std::vector<std::string>& tokens) const {

    // to store the trimmed tokens
    std::vector<std::string> trimmed_tokens;

    for (const auto& token : tokens) {
        size_t start = 0;
        size_t end = token.length();

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

// Returns true if line starts with start
bool Reader::LineStartsWith(const std::string& line, const std::string& start) const {
    if (line.size() < start.size()) {
        return false;
    }
    return line.substr(0, start.size()) == start;
}

// Returns true if line ends with end
bool Reader::LineEndsWith(const std::string& line, const std::string& end) const {
    if (line.size() < end.size()) {
        return false;
    }
    return line.substr(line.size() - end.size(), end.size()) == end;
}


// Interpret a key-value pair and update raw_data_ accordingly.
void Reader::InterpretKeyValuePair(const std::string& key, const std::string& value) {

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
    else if (key == "tolerance") {
        raw_data_.solver_params.tolerance = ParseDouble(value);
    }
    else if (key == "max_iterations") {
        raw_data_.solver_params.max_iterations = static_cast<size_t>(ParseDouble(value));
    }
    else if (key == "function") {
    size_t pos = 0;
    raw_data_.function_params.function_shape = ParseFunctionRecursive(value, pos, raw_data_.function_params.function_expressions);
    }
    else if(key == "derivative"){
        size_t pos = 0;
        raw_data_.function_params.derivative_shape = ParseFunctionRecursive(value, pos, raw_data_.function_params.derivative_expressions);
    }
    else if(key == "order"){
        raw_data_.solver_params.order = static_cast<int>(ParseDouble(value));
    }
    else if (key == "method"){
        
        raw_data_.solver_params.method = ParseSolverMethod(value);
    }
    else{
        // skip unknown keys
    }
    // Does not create Function yet, just stores expressions and shape
}


// Trim a single string
std::string Reader::TrimString(const std::string& str) const {
    size_t start = 0;
    size_t end = str.size();

    while (start < end && std::isspace( static_cast<char>(str[start]))) {
    start++;
    }

    while (end > start && std::isspace(static_cast<char>(str[end - 1]))) {
    end--;
    }

    return str.substr(start, end - start);
}


//------------------------------------------------------------//
// Private Method(s)
//------------------------------------------------------------//

// Parse solver method from string to enum
SolverMethod Reader::ParseSolverMethod(const std::string& method){
    if (method == "AdamMoulton") {
        return SolverMethod::kAdamMoulton;
    }
    else if (method == "AdamBashforth") {
        return SolverMethod::kAdamBashforth;
    }
    else if (method == "BackwardDifferentiation") {
        return SolverMethod::kBackwardDifferentiation;
    }
    else if (method == "BackwardEuler") {
        return SolverMethod::kBackwardEuler;
    }
    else if (method == "ForwardEuler") {
        return SolverMethod::kForwardEuler;
    }
    else if (method == "ForwardEulerLight") {
        return SolverMethod::kForwardEulerLight;
    }
    else if (method == "RungeKutta") {
        return SolverMethod::kRungeKutta;
    }
    else {
        // Unknown method, return undefined
        return SolverMethod::kUndefinedSolver;
    }
}

// Parses a tensor from its string representation.
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




// Recursive helper function to parse a tensor string into a flattened list of values and determine the shape.
// ChatGPT helped me a lot with this function.
template <typename T>
std::vector<size_t> Reader::ParseTensorRecursive(const std::string& str, size_t& pos, std::vector<T>& data)
{
    std::vector<size_t> shape;
    size_t element_count = 0; // number of elements in current dimension

    //assume first char is '['
    ++pos;

    while (pos < str.size()) {

        //Check if we start a new nested dimension
        if (str[pos] == '[') {

            std::vector<T> nested_data;
            std::vector<size_t> nested_shape = ParseTensorRecursive(str, pos, nested_data);

            data.insert(data.end(), nested_data.begin(), nested_data.end());

            if (element_count == 0) {
                // first time setting shape for this dimension
                shape = nested_shape;
            }

            else if (shape != nested_shape) {
                throw std::invalid_argument("Inconsistent tensor shape detected.");
            }

            // count the number of elements in this dimension
            ++element_count;
        }

        // End of this dimension if we see a closing bracket
        else if (str[pos] == ']') {
            ++pos;
            break;
        }

        // we skip commas and whitespace in the elements (they are not counted)
        else if (str[pos] == ',' || std::isspace(static_cast<unsigned char>(str[pos]))) {
            ++pos;
        }

        // We parse a value (complex or double)
        else {
    
            // find the end of value
            std::string value_str;
            int bracket_level = 0;

            while (pos < str.size()){
                if (str[pos] == '(') {
                    // complex number start
                    bracket_level++;
                }
                else if (str[pos] == ')') {
                    // complex number end
                    bracket_level--;
                }

                // Break if we reach a comma or closing bracket at the top level
                if ((str[pos] == ',' || str[pos] == ']') && bracket_level == 0) {
                    break;
                }

                value_str += str[pos];
                ++pos;
            }

            //Safety trim
            value_str = TrimString(value_str);

            // Convert the string into a value of type T
            T value;

            // Depending on the template type, parse accordingly.
            //constexpr allows if statements to be evaluated at compile time, so only the relevant branch is compiled.
            // We must have constexpr because the functions ParseDouble and ParseComplexNumber are not templated and only work for their specific types.
            //So the compiler would complain if both branches were compiled. with constexpr only the relevant branch is compiled.
            if constexpr (std::is_same_v<T, double>) {
                value = ParseDouble(value_str);
            }

            else if constexpr (std::is_same_v<T, DynamicTensor::Complex>) {
                if (!IsComplexNumber(value_str)) {
                    // This is entered for a string like [(1,2), 3, (4,5)] where 3 is a real number in a complex tensor
                    double real_part = ParseDouble(value_str);
                    value = DynamicTensor::Complex(real_part, 0.0);
                } else {
                    // Proper complex number
                    value = ParseComplexNumber(value_str);
                }
            }

            // Add the parsed value to the data vector
            data.push_back(value);
            ++element_count;
        }
    }

    // we only enter this part when we finish parsing a dimension

    // Compute the actual shape
    if (shape.empty()) {
        // Only one dimension, eg. [1,2,3], no nest parsing
        return { data.size() };
    }
    else {
        // size of the current dimension (shape already contains sizes of nested dimensions)
        shape.insert(shape.begin(), element_count);
        return shape;
    }
}

// A simple check for the strict format of complex numbers
bool Reader::IsComplexNumber(const std::string& str) const {
    
    // complex numbers must be represented as (a,b) where a is the real part and b is the imaginary part
    if (str.size() < 5){
        return false;
    }


    if (str.front() != '(' || str.back() != ')') {
        return false;
    }

    // remove parantheses
    std::string content = str.substr(1, str.size() - 2); 

    //split by comma and trim spaces
    auto parts = Split(content, ',');
    parts = Trim(parts);

    if (parts.size() != 2){
        return false;
    }

    return (IsNumeric(parts[0]) && IsNumeric(parts[1]));
}

// Parses a complex number from its string representation.
std::complex<double> Reader::ParseComplexNumber(const std::string& str) const {
    std::string trimmed_str = TrimString(str);

    if (!IsComplexNumber(trimmed_str)) {
        throw std::invalid_argument("Invalid complex number format: " + str);
    }

    // remove parentheses
    std::string content = trimmed_str.substr(1, trimmed_str.size() - 2); 
    auto parts = Split(content, ',');
    parts = Trim(parts);
    double real = std::stod(parts[0]);
    double imag = std::stod(parts[1]);

    return std::complex<double>(real, imag);
}

// Parses a double from its string representation.
double Reader::ParseDouble(const std::string& str) const {
    return std::stod(str);
}

// Checks if the string contains a complex number. This will look for any occurrence of (a,b) in the string.
bool Reader::HasComplexNumber(const std::string& str) const {

    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == '(') {

            // Look for the closing parenthesis. returns npos if not found
            size_t j = str.find(')', i);

            if (j != std::string::npos) {

                //Make substring and check if it's a complex number (could be somethig else in parantheses)
                std::string potential_complex = str.substr(i, j - i + 1);
                if (IsComplexNumber(potential_complex)) {

                    return true;
                }
                i = j; // Move index to the closing parenthesis
            }
        }
    }
    return false;
}

// Recursive helper function to parse a function string into a flattened list of expressions and determine the shape.
std::vector<size_t> Reader::ParseFunctionRecursive(const std::string& str, size_t& pos, std::vector<std::string>& flat_expressions) {
    
    std::vector<size_t> sub_shape;
    size_t element_count = 0;

    // assume first char is '['
    ++pos;

    while(pos < str.size()) {

        if (str[pos] == '[') {
            // Start of a new nested dimension
            std::vector<std::string> nested_expression;
            auto nested_shape = ParseFunctionRecursive(str, pos, nested_expression); //Recursive call on nested dimension

            flat_expressions.insert(flat_expressions.end(), nested_expression.begin(), nested_expression.end()); // Flatten expressions

            if (sub_shape.empty()) {
                sub_shape = nested_shape;
            }

            else if (sub_shape != nested_shape){
                throw std::invalid_argument("Inconsistent function shape detected.");
            }
            // count the number of elements in this dimension
            ++element_count;
        }

        else if (str[pos] == ']') {
            // End of the current dimension
            ++pos;
            break;
        }

        else if (str[pos] == ',' || std::isspace(str[pos])) {
            // Separator between subdimension elements, or if space, just move on
            ++pos;
        }
        else {
            // Parse a function expression
            std::string token;
            while (pos < str.size() && str[pos] != ',' && str[pos] != ']' && str[pos] != '[') {
                token += str[pos];
                ++pos;
            }
            // Safety trim
            token = TrimString(token);
            
            // Add the parsed expression to the flat_expressions vector
            flat_expressions.push_back(token);
            ++element_count;
        }
    }

    // we only enter this part when we finish parsing a dimension

    if (sub_shape.empty()) {
        // First dimension size
        return {flat_expressions.size()}; 
    }
    else {
       // size of the current dimension (sub_shape already contains sizes of nested dimensions)
        sub_shape.insert(sub_shape.begin(), element_count);
        return sub_shape;
    }
}

