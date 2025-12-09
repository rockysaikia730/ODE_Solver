#include <fstream>
#include <string>
#include "ode_raw_data.h"
#include "reader_txt.h"
#include "parsed_function.h"

TxtReader::TxtReader(const std::string& file_name, char separator, char inline_separator, char comment_char)
    : Reader(file_name, separator), inline_separator_(inline_separator), comment_char_(comment_char) {}


void TxtReader::Read() {
    if (!Open()) {
        // Handle error! MISSING IMPLEMENTATION
        throw std::runtime_error("Failed to open file: " + file_name_);
    }

    std::string line;

    while (std::getline(file_stream_, line)) {
        std::string trimmed = TrimString(line);
        if(trimmed.empty() || LineStartsWith(trimmed, std::string(1, comment_char_))) {
            continue; // Skip empty lines and comment lines
        }

        auto pairs = Split(trimmed, inline_separator_); // Split line into multiple key-value pairs
        pairs = Trim(pairs);

        for (const auto& pair : pairs) {
            auto tokens = Split(pair, separator_); // Split each pair into key and value
            tokens = Trim(tokens);

            if (tokens.size() != 2) {
                // Handle error! MISSING IMPLEMENTATION
                throw std::invalid_argument("Invalid key-value pair: " + pair);
            }

            std::string key = ToLower(TrimString(tokens[0]));
            std::string value = TrimString(tokens[1]);

            InterpretKeyValuePair(key, value);
        }
    }
    Close();

    if (raw_data_.function_params.function_expressions.empty()) {
        throw std::invalid_argument("No function expressions found in TXT file.");
    }
    //create function (copied code from CsvReader...)
    raw_data_.function = std::make_shared<ParsedFunction>(
        raw_data_.function_params.function_expressions,
        raw_data_.function_params.function_shape,
        raw_data_.function_params.derivative_expressions,
        raw_data_.function_params.derivative_shape
    );
}