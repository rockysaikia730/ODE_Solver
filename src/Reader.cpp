//
// Created by andra on 21/11/2025.
//

#include "Reader.h"
#include <fstream>

template<typename T>
bool Reader<T>::checkFileExist() const {
    bool result = true;
    if (!file.is_open()) {
        throw std::runtime_error("File does not exist");
    }

    return file.good();
}
template<typename T>
void Reader<T>::setFile(const std::string &newname) {
    bool result = false;
    
    file.open(newname);
    result = checkFileExist();

    if (result) {
        filename = newname;
    }
}

template<typename T>
Reader<T>::Reader(const std::string &newfile, const char &newsep) {
    bool result = false;

    file.open(newfile);
    result = checkFileExist();

    if (result) {
        filename = newfile;
        sep = newsep;
    }
}
