#ifndef READER_CSV_H
#define READER_CSV_H

#include <string>
#include <fstream>
#include "reader.h"
#include "ode_raw_data.h"

class CsvReader: public Reader {
public:
    CsvReader(const std::string& file_name, char separator = ',');
    virtual ~CsvReader() = default;

    virtual OdeRawData Read() override;
};
#endif // READER_CSV_H