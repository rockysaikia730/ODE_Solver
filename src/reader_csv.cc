#include <fstream>
#include "reader.h"
#include "reader_csv.h"
#include "ode_raw_data.h"

CsvReader::CsvReader(const std::string& file_name, char separator, bool has_header)
    : Reader(file_name, separator), has_header_(has_header) {}


void CsvReader::Read() {
    if (!Open()) {
        // Handle error! MISSING IMPLEMENTATION
    }

    std::string line;

    if (has_header_) {
        std::getline(file_stream_, line);
    }

    while (std::getline(file_stream_, line)) {
        // Trim entire line
        line = Trim(line);
        
        if(line.empty()) {
            continue; // Skip empty lines
        }
        auto tokens = Split(line, separator_);
        tokens = Trim(tokens);

        if (tokens.size() != 2) {
            // Handle error! MISSING IMPLEMENTATION
        }

        std::string key = ToLower(tokens[0]);
        std::string value = tokens[1];

        InterpretKeyValuePair(key, value);
    }

    Close();
}