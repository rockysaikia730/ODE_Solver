#ifndef READER_CSV_H
#define READER_CSV_H

#include <string>
#include <fstream>
#include "reader_file.h"
#include "../Data_structures/ode_raw_data.h"

class CsvReader: public FileReader {
public:
    CsvReader(const std::string& file_name, char separator = ',');
    virtual ~CsvReader() = default;

    virtual OdeRawData Read() override;
};
#endif // READER_CSV_H