#include "reader.h"
#include "reader_csv.h"
#include "reader_txt.h"
#include <iostream>

int main() {
    try {
        // Testing CSV Reader
        CsvReader csvReader("test_data.csv", ';', true);
        csvReader.Read();
        const OdeRawData& csvData = csvReader.GetRawData();
        std::cout << "CSV Data Read Successfully." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error reading CSV file: " << e.what() << std::endl;
    }

    try {
        // Testing TXT Reader
        TxtReader txtReader("test_data.txt");
        txtReader.Read();
        const OdeRawData& txtData = txtReader.GetRawData();
        std::cout << "TXT Data Read Successfully." << std::endl;
    } catch (const std::exception& e) {
        std::string msg = e.what();

        if (msg.rfind("Unknown key skipped:", 0) == 0) {
            std::cout << "[Warning] " << msg << std::endl;
        } else {
            std::cerr << "Error encountered while reading TXT file: " << msg << std::endl;
        }
    }

    std::cout << "Reader tests completed." << std::endl;
    return 0;
}
