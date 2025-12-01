#ifndef READER_FILE_H
#define READER_FILE_H

#include <string>
#include <fstream>
#include <vector>
#include "reader.h"

class FileReader : public Reader {
public:
    /**
     * @brief Constructor that initializes the FileReader with a file name.
     * @param file_name The name of the file to read data from.
     */
    FileReader(const std::string& file_name, char separator = ';');

    /**
     * @brief Virtual destructor.
     */
    virtual ~FileReader() = default;

    /**
     * @brief Opens the data source for reading.
     * @return True if the data source was successfully opened, false otherwise.
     */
    virtual bool Open();

    /**
     * @brief Closes the data source.
     */
    virtual void Close();

    /**
     * @brief Sets the separator character used in the file.
     * @param separator The separator character to set.
     */
    void SetSeparator(char separator);

    /**
     * @brief Gets the separator character used in the file.
     * @return The separator character.
     */
    char GetSeparator() const;


protected:
    /**
     * @brief File stream for reading data.
     */
    std::ifstream file_stream_;
    
    /**
     * @brief Character used as a separator in the file.
     */
    char separator_;

    /**
     * @brief Splits a string into tokens based on a delimiter.
     * @param str The string to split.
     * @return A vector of tokens.
     */
    virtual std::vector<std::string> Split(const std::string& str);

    /**
     * @brief Trims whitespace from each token if there is a space at the beginning or end of the token. It should not remove spaces within the token, as those may be vector values.
     * @param tokens The vector of strings to trim.
     * @return A vector of trimmed strings.
     */
    virtual std::vector<std::string> Trim(const std::vector<std::string>& tokens);
};



#endif // READER_FILE_H