#ifndef FUNCTION_PARSER_H
#define FUNCTION_PARSER_H
#include <string>
#include <memory>
#include "expression.h"

/** 
 * @brief Function Parser Class.
 * Parses and holds a mathematical expression along with its position in input.
 * To prepare this function parser, I relied heavily on ChatGPT for help with parsing logic and techniques.
 * The parsing methods are designed to handle different components of mathematical expressions.
 * The strict formatting is assumed for the input expressions, see the README for more details.
 */
class FunctionParser {
public:
    /** 
     * @brief Constructor for FunctionParser.
     * 
     * @param expr A unique pointer to the root Expression object.
     * @param pos The position of the expression in the input string.
     */
    FunctionParser(const std::string& expr, size_t pos)
        : expression_(expr), position_(pos) {}

    /**
     * @brief Get the parsed expression.
     * @return A pointer to the root Expression object.
     */
    size_t GetPosition() const;

    
private:
    std::string expression_;
    size_t position_;

    /**
     * @brief Skip Whitespaces.
     * Advances the position to skip any whitespace characters.
     */
    void SkipWhitespaces();

    /**
     * @brief Look at the current character.
     * @param expected The expected character to look for.
     * @return True if the current character matches the expected one, false otherwise.
     * Does not advance the position.
     */
    bool Look(char expected) const;
    /**
     * @brief Look if the current character matches the expected one.
     * @param expected The expected character to match.
     * @return True if matched, false otherwise, and advances the position.
     */
    bool Match(char expected);

    /**
     * @brief Return the current character without advancing the position.
     * @return The current character.
     */
    char Peek() const;

    /**
     * @brief Parse a mathematical expression from input.
     * @return A unique pointer to the root Expression object.
     */
    std::unique_ptr<Expression> ParseExpression();

    /** 
     * @brief Parse Term.
     * Handles addition and subtraction.
     */
    std::unique_ptr<Expression> ParseTerm();

    /** 
     * @brief Parse Factor.
     * Handles multiplication and division.
     */
    std::unique_ptr<Expression> ParseFactor();
    /** 
     * @brief Parse Primary.
     * Handles parentheses, numbers, variables, and functions.
     */
    std::unique_ptr<Expression> ParsePrimary();
    
    /** 
     * @brief Parse Function.
     * Handles function calls.
     */
    std::unique_ptr<Expression> ParseFunction();
    
    /** 
     * @brief Parse Number.
     * Handles numeric literals.
     */
    std::unique_ptr<Expression> ParseNumber();
    
    /**
     * @brief Parse Variable.
     * Handles variable identifiers.
     */
    std::unique_ptr<Expression> ParseVariable();

    /**
     * @brief Parse Size and shape information from input.
     * @return A vector of size_t representing the parsed size.
     */
    std::vector<size_t>  ParseIndexList();

};
#endif // FUNCTION_PARSER_H