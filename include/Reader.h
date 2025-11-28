//
// Created by andra on 21/11/2025.
//




#ifndef INCLUDE_READER_H_
#define INCLUDE_READER_H_

/**
 * @file reader.h
 * @brief Defines the abstract Reader class template for loading ODE input data.
 */

#include <string>
#include <functional>
#include <variant>
#include <fstream>
#include "TableEntry.h"
/**
 * @brief Abstract base class for reading ODE input data from files.
 *
 * This class defines a common interface used by all reader types 
 * (e.g., CSV reader, TXT reader, JSON reader).
 * 
 * The Reader:
 *   - Stores a filename and a field separator character.
 *   - Provides functions to check file existence and file extension.
 *   - Returns parsed data as either a Function or a TableEntry<T>.
 *
 * @tparam T Scalar type of the ODE state (e.g., double, std::complex<double>).
 */
template <typename T>
class Reader {
protected:
    /// Convenience alias for ODE state vector.
    using State = std::vector<T>;

    /**
     * @brief Callable representing the RHS function f(t, y).
     *
     * The signature State(double t, const State& y) must return the derivative vector.
     */
    using Function = std::function<State(double, const State&)>;

    /**
     * @brief Variant of possible readable data types.
     *
     * A Reader may return:
     *   - A Function (parsed from file or predefined)
     *   - A TableEntry<T> (for table-based or tabulated data)
     */
    using Data = std::variant<Function, TableEntry<T>>;

    /// Field separator used when parsing (e.g., ',', ';', ' ').
    char sep_;

    /// Name of the file to read from.
    std::string filename_;

    /// File stream used internally for reading.
    std::ifstream file_;

    /**
     * @brief Checks whether the file extension matches what this Reader supports.
     *
     * Pure virtual so each subclass implements its own check.
     *
     * @return true if the extension is acceptable, false otherwise.
     */
    virtual bool CheckExtension() const = 0;

public:
    /**
     * @brief Virtual destructor for proper polymorphic cleanup.
     */
    virtual ~Reader() = default;

    /**
     * @brief Checks whether the file exists and can be opened.
     *
     * @return true if file exists and is readable, false otherwise.
     */
    virtual bool CheckFileExist() const;

    /**
     * @brief Read and parse the file.
     *
     * Subclasses implement the interpretation of file contents
     * and return either:
     *   - A Function (parsed expression or compiled functor)
     *   - A TableEntry<T> (parsed numeric table)
     *
     * @return A Data variant containing parsed information.
     */
    virtual Data Read() = 0;

    /**
     * @brief Construct a Reader object.
     *
     * @param filename Name of the file to read.
     * @param sep Field separator character.
     */
    Reader(const std::string& filename, const char& sep);

    /**
     * @brief Change the file to be read.
     *
     * Resets the filename and closes any previously opened file.
     *
     * @param new_name New file name.
     */
    void SetFile(const std::string& new_name);

    /**
     * @brief Set the character used as field separator.
     *
     * @param new_sep Separator character.
     */
    void SetFileSeparator(const char& new_sep) { sep_ = new_sep; }

    /**
     * @brief Get the current file name.
     *
     * @return The stored filename.
     */
    std::string GetFileName() const { return filename_; }

    /**
    * @brief Get a const reference to the file stream.
    *
    * @return Const reference to the internal std::ifstream.
    */
    const std::ifstream& GetFile() const { return file_; }

    /**
     * @brief Get the current field separator character.
     *
     * @return Separator character.
     */
    char GetChar() const { return sep_; }
};
#endif // INCLUDE_READER_H_
