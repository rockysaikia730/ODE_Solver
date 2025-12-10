#ifndef OUTPUT_TXT_H_
#define OUTPUT_TXT_H_
#include <string>
#include <vector>
#include "output.h"
#include "ode_solver.h"


/**
 * @brief Derived class for outputting ODE solver results in TXT format.
 */
class OutputTxt : public Output {
public:
    /**
     * @brief Constructor.
     * 
     * @param filename The name of the TXT output file.
     * @param separator The separator character used in the TXT file.
     * @param comment_char The character used to denote comments in the TXT file.
     */
    OutputTxt(const std::string& filename, char separator = ' ', char comment_char = '#');

    /**
     * @brief Overrides the Write method to output results in TXT format.
     * 
     * @param solver A constant reference to the OdeSolver instance containing results to output.
     */
    void Write(const OdeSolver& solver) override;

    /**
     * @brief Gets the comment character used in the TXT file.
     */
    char GetCommentChar() const;
    
    /**
     * @brief Sets the comment character used in the TXT file.
     * @param comment_char The new comment character.
     */
    void SetCommentChar(char comment_char);

    /**
     * @brief Gets the header lines of the TXT file.
     */
    std::vector<std::string> GetHeader() const;

    /**
     * @brief Sets the header lines of the TXT file.
     * @param header The new header lines.
     */
    void SetHeader(const std::vector<std::string>& header);

    private:
    /**
     * @brief The character used to denote comments in the TXT file.
     */
    char comment_char_;
    
    /**
     * @brief Header lines to the TXT file.
     */
    std::vector<std::string> header_;

};

#endif // OUTPUT_TXT_H