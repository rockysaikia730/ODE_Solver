#ifndef READER_TXT_H
#define READER_TXT_H

#include <string>
#include <fstream>
#include "reader_file.h"
#include "../Data_structures/ode_raw_data.h"

class TxtReader: public FileReader {
public:
    TxtReader(const std::string& file_name, char separator = '\t');
    virtual ~TxtReader() = default;

    virtual OdeRawData Read() override;
};

#endif // READER_TXT_H