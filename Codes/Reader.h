//
// Created by andra on 21/11/2025.
//

#ifndef CODES_READER_H
#define CODES_READER_H
#include <string>
#include <functional>
#include <variant>

template <typename T>
class Reader{
protected:
    using State = std::vector<T>;
    struct TableEntry {
        double t;
        State y;
        State f;
    };
    char sep;
    std::string filename;
    using Function = std::function<State(double, const State& )>;
    using data = std::variant<Function, TableEntry>;
    virtual bool checkFileName() const;

public:
    Reader(std::string, char);
    virtual ~Reader();
    virtual bool checkFileExist() const;
    virtual data Read() = 0;

    void setFileName(const std::string &newname) {filename = newname;};
    void setFileSeparator(const char &newsep) {sep = newsep;};
    std::string getFileName() const{ return filename;}
    char getChar() const  { return sep;}

};
#endif //CODES_READER_H