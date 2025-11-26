//
// Created by andra on 21/11/2025.
//

#ifndef CODES_READER_H
#define CODES_READER_H
#include <string>
#include <functional>
#include <variant>
#include <fstream>
#include "TableEntry.h"

template <typename T>
class Reader{
protected:
    using State = std::vector<T>;
    using Function = std::function<State(double, const State& )>;
    using data = std::variant<Function, TableEntry<T>>;
    char sep;
    std::string filename;
    std::ifstream file;

    virtual bool checkExtension() const = 0;

public:
    virtual ~Reader() = default;
    virtual bool checkFileExist() const;
    virtual data Read() = 0;

    Reader(const std::string &filename,const char &sep);

    void setFile(const std::string &newname);
    void setFileSeparator(const char &newsep) {sep = newsep;};
    std::string getFileName() const{ return filename;}
    char getChar() const  { return sep;}

};
#endif //CODES_READER_H