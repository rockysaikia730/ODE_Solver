#include <fstream>
#include "reader.h"
#include "reader_csv.h"
#include "ode_raw_data.h"
#include "parsed_function.h"
CsvReader::CsvReader(const std::string& file_name, char separator, bool has_header)
    : Reader(file_name, separator), has_header_(has_header) {}


void CsvReader::Read() {
    if (!Open()) {
        // Handle error! MISSING IMPLEMENTATION
        throw std::runtime_error("Failed to open file: " + file_name_);
    }

    std::string line;

    if (has_header_) {
        std::getline(file_stream_, line);
    }

    while (std::getline(file_stream_, line)) {
        // Trim entire line
        line = TrimString(line);
        
        if(line.empty()) {
            continue; // Skip empty lines
        }
        auto tokens = Split(line, separator_);
        tokens = Trim(tokens);

        if (tokens.size() != 2) {
            // Handle error! MISSING IMPLEMENTATION
            throw std::invalid_argument("Invalid key-value pair: " + line);
        }

        std::string key = ToLower(TrimString(tokens[0]));
        std::string value = TrimString(tokens[1]);

        InterpretKeyValuePair(key, value);
    }

    Close();

    if (raw_data_.function_params.function_expressions.empty()) {
        throw std::invalid_argument("No function expressions found in CSV file.");
    }

    //create function
    raw_data_.function = std::make_shared<ParsedFunction>(
        raw_data_.function_params.function_expressions,
        raw_data_.function_params.function_shape,
        raw_data_.function_params.derivative_expressions,
        raw_data_.function_params.derivative_shape
    );
    
}