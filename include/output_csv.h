#ifndef OUTPUT_CSV_H_
#define OUTPUT_CSV_H_
#include <vector>
#include <string>
#include "output.h"


/**
 * @brief Derived class for outputting ODE solver results in CSV format.
 * The output allows a customizable separator character, which can be set through the
 * SetSeparator method or the constructor. A header row can also be specified using the
 * SetHeader method.
 */
class OutputCsv : public Output {
public:
    /**
     * @brief Constructor.
     * @param filename The name of the CSV output file.
     * @param separator The separator character used in the CSV file.
     */
    OutputCsv(const std::string& filename, char separator = ';', char element_separator = ',');

    /**
     * @brief Gets the separator character used in the CSV file.
     */
    char GetSeparator() const;

    /**
     * @brief Sets the separator character used in the CSV file.
     * @param separator The new separator character.
     */
    void SetSeparator(char separator);

    /**
     * @brief Gets the header row of the CSV file.
     */
    std::vector<std::string> GetHeader() const;

    /**
     * @brief Sets the header row of the CSV file.
     * @param header The new header string.
     */
    void SetHeader(const std::vector<std::string>& header);

protected:
    /**
     * @brief File specific setup for CSV output. We write the header row here.
     * @param file The output file stream.
     */
    void FileSpecificSetup(std::ofstream& file) const override;
    
private:
    /**
     * @brief A header row to the CSV file.
     */
    std::vector<std::string> header_;
};

#endif // OUTPUT_CSV_H_