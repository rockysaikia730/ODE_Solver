#include "reader_txt.h"

TxtReader::TxtReader(const std::string& file_name, char separator)
    : FileReader(file_name, separator) {}

OdeRawData TxtReader::Read() {
    OdeRawData raw_data;
    bool is_open = Open();
    if (!is_open) {
        // Handle error: could not open file
        return raw_data;
    }
    //MISSING IMPLEMENTATION
    Close();

    return raw_data;
}