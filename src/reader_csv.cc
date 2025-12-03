#include <fstream>
#include "reader_csv.h"
#include "ode_raw_data.h"

CsvReader::CsvReader(const std::string& file_name, char separator)
    : Reader(file_name, separator) {}


OdeRawData CsvReader::Read() {

    OdeRawData raw_data;
    bool is_open = Open();
    if (!is_open) {
        // Handle error: could not open file
        return raw_data;
    }

    // strict format: no header, then match key 'separator' value
    std::string line;

    //Missing implementation

}