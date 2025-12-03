#ifndef READER_H
#define READER_H

#include <string>
#include <fstream>
#include "dynamic_tensor.h"
#include "ode_raw_data.h"

/**
 * @class Reader
 * @brief A class responsible for reading and parsing data from various sources.
 *
 * The Reader class provides methods to read data from files, databases, or other input streams,
 * and convert them into DynamicTensor objects for further processing.
 */
class Reader {
public:
    /**
     * @brief Constructor that initializes the Reader with a file name.
     * @param file_name The name of the file to read data from.
     */
    Reader(const std::string& file_name, char separator = ';');
    
    /**
     * @brief Virtual destructor.
     */
    virtual ~Reader() = default;

    /**
     * @brief Reads data from the source and returns it as an OdeRawData object.
     * @return sets the raw_data_ member variable.
     */
    virtual void Read() = 0;


    /**
     * @brief Get the file name associated with the Reader.
     * @return The file name as a string.
     */
    std::string GetFileName() const;
    
    /**
     * @brief Get the file extension associated with the Reader.
     * @return The file extension as a string.
     */
    std::string GetFileExtension() const;

    /**
     * @brief Set the file name for the Reader.
     * @param file_name The new file name to set.
     */
    void SetFileName(const std::string& file_name);

    /**
     * @brief Set the file extension for the Reader.
     * @param file_extension The new file extension to set.
     */
    void SetFileExtension(const std::string& file_extension);

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

    /**
     * @brief Parser to convert a string into a DynamicTensor.
     * @param str The string to parse.
     * @return A DynamicTensor object.
     */

    /**
     * @brief Get the raw data read by the Reader.
     * @return An OdeRawData object containing the read data.
     */
    OdeRawData GetRawData() const {
        return raw_data_;
    }

protected:
    /**
     * @brief The name of the file to read from.
     */
    std::string file_name_;

    /**
     * @brief The file extension of the file to read from.
     */
    std::string file_extension_;

    /**
     * @brief The separator character used in the file.
     */
    char separator_;

    /**
     * @brief The input file stream.
     */
    std::ifstream file_stream_;

    /**
     * @brief Container for all read data.
     */
    OdeRawData raw_data_;

    /**
     * @brief Checks if a string represents a numeric value.
     * @param str The string to check.
     * @return True if the string is numeric, false otherwise.
     */
    bool IsNumeric(const std::string& str) const;

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

    /**
     * @brief Helper function to parse a string into a DynamicTensor.
     * @param str The string to parse.
     * @return A DynamicTensor object.
     */
    DynamicTensor ParseTensor(const std::string& str);

    /**
     * @brief Helper function to check if tring is a complex number or a double.
     * @param str The string to check.
     * @return True if the string is complex, false if double.
     */
    bool IsComplexNumber(const std::string& str) const;

    /**
     * @brief Create a function and save the shared pointer to it in raw_data_.func.
     * @param func_str The string representing the function.
     */
    void ParseFunction(const std::string& func_str);

    /**
     * @brief Converts a string to lowercase.
     * @param str The string to convert.
     * @return The lowercase version of the string.
     */
    std::string ToLower(const std::string& str) const;

    /**
     * @brief Reads a line from the file stream.
     * @return The line read from the file.
     */
    std::string ReadLine();

    /**
     * @brief Reads next non-empty line from the file stream.
     * @return The next non-empty line read from the file.
     */
    std::string ReadNextNonEmptyLine();

    /**
     * @brief Returns the starting value of line
     */
    bool LineStartsWith(const std::string& line, const std::string& start) const;

    /**
     * @brief Returns the ending value of line
     */
    bool LineEndsWith(const std::string& line, const std::string& end) const;

    /**
     * @brief Removes whitespace and new line characters from both ends of a string.
     */
    std::string TrimString(const std::string& str) const;

    /**
     * @brief Tokenise a string based on the separator character.
     */
    virtual std::vector<std::string> Tokenise(const std::string& str) = 0;

    /**
     * @brief Parse a double value from a string.
     */
    double ParseDouble(const std::string& str) const;

    /**
     * @brief Parse an integer value from a string.
     */
    int ParseInt(const std::string& str) const;

    /**
     * @brief Parse a complex value from a string.
     */
    std::complex<double> ParseComplex(const std::string& str) const;
};

#endif // READER_H