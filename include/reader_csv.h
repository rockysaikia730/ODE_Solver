#ifndef READER_CSV_H
#define READER_CSV_H

#include <string>
#include <fstream>
#include "reader.h"
#include "ode_raw_data.h"

/**
 * @class CsvReader
 * @brief A class for reading and parsing CSV (Comma-Separated Values) files.
 * Inherits from the Reader base class. Supports optional header row, which can be skipped during reading.
 */
class CsvReader: public Reader {
public:

    /**
     * @brief Constructor that initializes the CsvReader with a file name, separator, and header option.
     * @param file_name The name of the CSV file to read data from.
     * @param separator The character used to separate values in the CSV file. Default is ','.
     * @param has_header Boolean indicating whether the CSV file has a header row. Default is false.
     */
    CsvReader(const std::string& file_name, char separator = ',', bool has_header = false);
    
    /**
     * @brief Virtual destructor.
     */
    virtual ~CsvReader() = default;

    /**
     * @brief Reads data from the CSV file and populates the raw_data_ member variable.
     */
    virtual void Read() override;
    
private:

    /**
     * @brief Indicates whether the CSV file has a header row.
     */
    bool has_header_;
};
#endif // READER_CSV_H