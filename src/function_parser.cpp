#include <string>
#include <memory>
#include "expression.h"
#include "function_parser.h"

size_t FunctionParser::GetPosition() const {
    return position_;
}

void FunctionParser::SkipWhitespaces() {
    while (position_ < expression_.size() && isspace(expression_[position_])) {
        ++position_;
    }
}

char FunctionParser::Peek() const {
    if (position_ < expression_.size()) {
        return expression_[position_];
    }
    return '\0'; // End of string
}

bool FunctionParser::Look(char exptected) const {
    return (position_ < expression_.size() && expression_[position_] == exptected);
}

bool FunctionParser::Match(char expected) {
    if (Look(expected)) {
        ++position_;
        return true;
    }
    return false;
}

std::unique_ptr<Expression> FunctionParser::ParseExpression() {
    auto term = ParseTerm();
    SkipWhitespaces();
    while(Look('+') || Look('-')) {
        char op = Peek(); // operator at current position
        Match(op); // advance position
        SkipWhitespaces(); // In case of spaces between terms
        auto next_term = ParseTerm();

        term = std::make_unique<BinaryExpression>(op, std::move(term), std::move(next_term));
        SkipWhitespaces();
    }
    return term;
}

std::unique_ptr<Expression> FunctionParser::ParseTerm() {
    auto factor = ParseFactor();
    SkipWhitespaces();
    while(Look('*') || Look('/')) {
        char op = Peek(); // operator at current position
        Match(op); // advance position
        SkipWhitespaces(); // In case of spaces between factors
        auto next_factor = ParseFactor();

        factor = std::make_unique<BinaryExpression>(op, std::move(factor), std::move(next_factor));
        SkipWhitespaces();
    }
    return factor;
}

std::unique_ptr<Expression> FunctionParser::ParseFactor() {
    auto primary = ParsePrimary();
    SkipWhitespaces();
    if (Look('^')) {
        char op = Peek(); // operator at current position
        Match(op); // advance position
        SkipWhitespaces(); // In Case of spaces between base and exponent
        auto exponent = ParseFactor(); // Right associative

        primary = std::make_unique<BinaryExpression>(op, std::move(primary), std::move(exponent));
    }
    return primary;
}

std::unique_ptr<Expression> FunctionParser::ParsePrimary() {
    SkipWhitespaces();
    if (Match('(')) {
        auto expr = ParseExpression();
        SkipWhitespaces();
        if (!Match(')')) {
            //MISSING ERROR HANDLING
            // Expected ')'
        }
        return expr;
    }
    else if (std)
    return nullptr; // Placeholder
}

std::unique_ptr<Expression> FunctionParser::ParseFunction() {
    auto start = position_;
    while(position_ < expression_.size() && isalpha(expression_[position_])) {
        //skips to the end of the function name (isalpha = alphabetic character)
        ++position_;
    }
    std::string func_name = expression_.substr(start, position_ - start);
    SkipWhitespaces();
    if (!Match('(')) {
        //MISSING ERROR HANDLING
        // Expected '(' after function name
    }
    auto arg = ParseExpression();
    SkipWhitespaces();
    if (!Match(')')) {
        //MISSING ERROR HANDLING
        // Expected ')' after function argument
    }
    return std::make_unique<FunctionExpression>(func_name, std::move(arg));
}

std::unique_ptr<Expression> FunctionParser::ParseNumber() {
    size_t start = position_;
    while (position_ < expression_.size() && (isdigit(expression_[position_]) || expression_[position_] == '.')) {
        ++position_;
    }
    std::string number_str = expression_.substr(start, position_ - start);
    double value = std::stod(number_str);
    return std::make_unique<NumberExpression>(value);
}
std::unique_ptr<Expression> FunctionParser::ParseVariable() {
    size_t start = position_;
    if (Match('t')) {
        return std::make_unique<VariableExpression>("t");
    } else if (Match('y')) {
        if (!Match('{')){
            //HANDLE ERROR
        }
        auto idx = ParseIndexList(); // Move position and get indices of y
        if (!Match('}')){
            //HANDLE ERROR
        }
        return std::make_unique<YExpression>(idx);
    }
    return nullptr; // Placeholder
}


std::vector<size_t> FunctionParser::ParseIndexList() {
    std::vector<size_t> sizes;
    while (true) {
        SkipWhitespaces();
        size_t start = position_;
        while (position_ < expression_.size() && isdigit(expression_[position_])) {
            // Checking for digits for size. Assumed to be integers, no handling for errors here
            ++position_;
        }
        std::string size_str = expression_.substr(start, position_ - start);
        sizes.push_back(static_cast<size_t>(std::stoul(size_str)));
        SkipWhitespaces();
        if (!Match(',')) {
            break;
        }
    }
    return sizes;
}


