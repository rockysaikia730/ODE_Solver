#include <fstream>
#include "reader.h"
#include "reader_csv.h"
#include "ode_raw_data.h"
#include "parsed_function.h"

//------------------------------------------------------------//
// Constructor(s)
//------------------------------------------------------------//
CsvReader::CsvReader(const std::string& file_name, char separator, bool has_header)
    : Reader(file_name, separator), has_header_(has_header) {}

//------------------------------------------------------------//
// Public Method(s)
//------------------------------------------------------------//

// Reads data from the CSV file and populates the raw_data_ member variable.
void CsvReader::Read() {

    // Open the file
    if (!Open()) {
        throw std::runtime_error("Failed to open file: " + file_name_);
    }

    std::string line;

    // If there's a header, read and discard it
    if (has_header_) {
        std::getline(file_stream_, line);
    }

    while (std::getline(file_stream_, line)) {

        // Trim entire line
        line = TrimString(line);
        
        // Skip empty lines
        if(line.empty()) {
            continue; 
        }

        // prepare key-value pair
        auto tokens = Split(line, separator_);
        tokens = Trim(tokens);

        if (tokens.size() != 2) {
            throw std::invalid_argument("Invalid key-value pair: " + line);
        }

        // interpret key-value pair
        std::string key = TrimString(tokens[0]);
        std::string value = TrimString(tokens[1]);

        InterpretKeyValuePair(key, value);
    }

    // Close the file
    Close();

    if (raw_data_.function_params.function_expressions.empty()) {
        throw std::invalid_argument("No function expressions found in CSV file.");
    }

    //create function (copied code from TxtReader...)
    raw_data_.function = std::make_shared<ParsedFunction>(
        raw_data_.function_params.function_expressions,
        raw_data_.function_params.function_shape,
        raw_data_.function_params.derivative_expressions,
        raw_data_.function_params.derivative_shape
    );
    
}