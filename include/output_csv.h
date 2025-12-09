#ifndef OUTPUT_CSV_H_
#define OUTPUT_CSV_H_
#include <vector>
#include <string>
#include "output.h"


/**
 * @brief Derived class for outputting ODE solver results in CSV format.
 */
class OutputCsv : public Output {
public:
    /**
     * @brief Constructor.
     * 
     * @param filename The name of the CSV output file.
     */
    OutputCsv(const std::string& filename, char separator = ';');

    /**
     * @brief Overrides the Write method to output results in CSV format.
     * 
     * @param solver A constant reference to the OdeSolver instance containing results to output.
     */
    void Write(const OdeSolver& solver) override;

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

private:
    /**
     * @brief A header row to the CSV file.
     */
    std::vector<std::string> header_;
};

#endif // OUTPUT_CSV_H_