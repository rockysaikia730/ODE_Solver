#ifndef READER_TXT_H
#define READER_TXT_H

#include <string>
#include <fstream>
#include "reader.h"
#include "ode_raw_data.h"

class TxtReader: public Reader {
public:
    /**
     * @brief Constructor for TxtReader.
     * @param file_name The name of the text file to read from.
     * @param separator The character that separates key-value pairs.
     * @param inline_separator The character that separates multiple key-value pairs in a single line.
     * @param comment_char The character that indicates the start of a comment line.
     */
    TxtReader(const std::string& file_name, char separator = '=', char inline_separator = ';', char comment_char = '#');
    
    /**
     * @brief Virtual destructor.
     */
    virtual ~TxtReader() = default;

    /**
     * @brief Reads data from the text file and populates the OdeRawData structure.
     * Comments lines starting with comment_char are ignored.
     * Multiple key-value pairs can be present in a single line, separated by inline_separator.
     */
    virtual void Read() override;
private:
    char comment_char_;
    char inline_separator_;
};

#endif // READER_TXT_H