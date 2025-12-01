#ifndef READER_H
#define READER_H

#include <string>
#include "../Data_structures/dynamic_tensor.h"
#include "../Data_structures/ode_raw_data.h"

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
    Reader(const std::string& file_name);
    
    /**
     * @brief Virtual destructor.
     */
    virtual ~Reader() = default;

    /**
     * @brief Reads data from the source and returns it as an OdeRawData object.
     * @return An OdeRawData object containing the read data.
     */
    virtual OdeRawData Read() = 0;


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

protected:
    std::string file_name_;
    std::string file_extension_;

    /**
     * @brief Checks if a string represents a numeric value.
     * @param str The string to check.
     * @return True if the string is numeric, false otherwise.
     */
    bool IsNumeric(const std::string& str) const;
};

#endif // READER_H