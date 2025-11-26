//
// Created by andra on 21/11/2025.
//

#include "Reader.h"
#include <fstream>

template<typename T>
bool Reader<T>::checkFileName() const {
    bool result = false;

    return result;
}

template<typename T>
bool Reader<T>::checkFileExist() const {
    bool result = true;
    if (!) {
        throw std::runtime_error("File does not exist");
    }

    return file.good();
}

template<typename T>
Reader<T>::Reader(const std::string &newfile,const char &newsep) {
    bool result = false;

    result = checkFileName();
    file.open(newfile);
    result = checkFileExist();

    if (result) {
        filename = newfile;
        sep = newsep;
    }

}
