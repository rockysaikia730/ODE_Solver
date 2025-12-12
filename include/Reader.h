#ifndef READER_H
#define READER_H

#include <string>
#include <fstream>
#include <complex>
#include <vector>
#include <memory>
#include "dynamic_tensor.h"
#include "ode_raw_data.h"
#include "solver_methods.h"


/**
 * @class Reader
 * @brief A class responsible for reading and parsing data from various sources. For the time being, only file reading is supported. NOte that the readers create the ODERawData object, as well as the function object.
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
     * @brief Get the raw data read by the Reader.
     * @return An OdeRawData object containing the read data.
     */
    const OdeRawData& GetRawData() const {
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
     * @brief Splits a string into tokens based on a delimiter.
     * @param str The string to split.
     * @param separator The character to use as the delimiter.
     * @return A vector of tokens.
     */
    std::vector<std::string> Split(const std::string& str, const char& separator) const;

    /**
     * @brief Trims whitespace from each token if there is a space at the beginning or end of the token. It should not remove spaces within the token, as those may be vector values.
     * @param tokens The vector of strings to trim.
     * @return A vector of trimmed strings.
     */
    std::vector<std::string> Trim(const std::vector<std::string>& tokens) const;

    /**
     * @brief Returns true if line starts with start
     */
    bool LineStartsWith(const std::string& line, const std::string& start) const;

    /**
     * @brief Returns true if line ends with end
     */
    bool LineEndsWith(const std::string& line, const std::string& end) const;

    /**
     * @brief trim a single string
     * @param str The string to trim.
     * @return The trimmed string.
     * This method removes leading and trailing whitespace from the input string.
     */
    std::string TrimString(const std::string& s) const;

    /**
     * @brief Key-value pair interpreter for reading lines.
     * @param key The key string.
     * @param value The value string.
     * This method interprets the key-value pairs and updates the raw_data_ member variable accordingly.
     */
    void InterpretKeyValuePair(const std::string& key, const std::string& value);

private:
    /**
     * @brief Parse the solver method from string to enum and set it in raw_data_
     * @param method_str The method string to parse.
     * @return The corresponding SolverMethod enum value.
     */
    SolverMethod ParseSolverMethod(const std::string& method_string);

    /**
    * @brief Recursive helper function to parse a tensor from a string. Only works for numeric types (double, complex).
    * @param str The string to parse.
    * @param pos The current position in the string.
    * @param data The vector to store the parsed data. This will be filled with values for y0.
    * @return A vector of size_t representing the shape of the parsed tensor.
    */
    template <typename T>
    std::vector<size_t> ParseTensorRecursive(const std::string& str, size_t& pos, std::vector<T>& data);
    
    /**
    * @brief Checks if a whole string has a complex number in it
    * @param str The string to check.
    * @return True if the string has a complex number, false otherwise.
    */
    bool HasComplexNumber(const std::string& str) const;

    /**
    * @brief Recursive helper function to parse a function string into a flattened list of expressions and determine the shape.
    * @param str The function string to parse.
    * @param pos The current position in the string.
    * @return size_t representing the shape of the parsed function.
    */
    std::vector<size_t> ParseFunctionRecursive(const std::string& str, size_t& pos, std::vector<std::string>& flat_expressions);

    /**
    * @brief Helper function to parse a string into a DynamicTensor.
    * @param str The string to parse.
    * @return A DynamicTensor object.
    */
    DynamicTensor ParseTensor(const std::string& str);

    /**
    * @brief Helper function to check if string is a complex number or a double.
    * @param str The string to check.
    * @return True if the string is complex, false if double.
    */
    bool IsComplexNumber(const std::string& str) const;

    /**
     * @brief Parses a string representing a complex number in the form (a,b).
     * @param str The string to parse.
     * @return A std::complex<double> object.
     */
    std::complex<double> ParseComplexNumber(const std::string& str) const;

    /**
     * @brief Parses a string representing a double.
     * @param str The string to parse.
     * @return A double value.
     */
    double ParseDouble(const std::string& str) const;

    /**
     * @brief Checks if a string represents a numeric value.
     * @param str The string to check.
     * @return True if the string is numeric, false otherwise.
     */
    bool IsNumeric(const std::string& str) const;

};

#endif // READER_H