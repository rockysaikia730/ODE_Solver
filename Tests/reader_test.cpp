#include "reader.h"
#include "reader_csv.h"
#include "reader_txt.h"
#include "ode_raw_data.h"
#include "parsed_function.h"
#include "dynamic_tensor.h"
#include "ode.h"
#include <iostream>
#include <memory>

int main() {
    std::cout << "==============================" << std::endl;
    std::cout << "Starting Reader tests..." << std::endl;
    
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

    std::cout << "==============================" << std::endl;
    std::cout << "Testing ODE construction" << std::endl;

        try {
            CsvReader csvReader("test_data.csv", ';', true);
            csvReader.Read();
            std::cout << "Reader y0 IsComplex: " << std::boolalpha 
            << csvReader.GetRawData().y0.IsComplex() << "\n";
            Ode odeFromCsv(csvReader);
            std::cout << "ODE constructed successfully from CSV data." << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error constructing ODE from CSV data: " << e.what() << std::endl;
        }

        try{
            TxtReader txtReader("test_data.txt");
            txtReader.Read();
            Ode odeFromTxt(txtReader);
            std::cout << "ODE constructed successfully from TXT data." << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error constructing ODE from TXT data: " << e.what() << std::endl;
        }
        
        std::cout << "the ODE, evaluated at t0 and y0 gives: " << std::endl;
        try {
            CsvReader csvReader("test_data.csv", ';', true);
            csvReader.Read();
            Ode odeFromCsv(csvReader);
            DynamicTensor result = odeFromCsv.Evaluate(odeFromCsv.GetTimeIn(), odeFromCsv.GetCondIn());
            std::cout << "ODE evaluated successfully at t0 and y0." << std::endl;
            std::cout << "Result: ";
            std::cout << result.at<DynamicTensor::Complex>({0}) << std::endl;
        }

        catch (const std::exception& e) {
            std::cerr << "Error evaluating ODE at t0 and y0: " << e.what() << std::endl;
        }

        try {
            TxtReader txtReader("test_data.txt");
            txtReader.Read();
            Ode odeFromTxt(txtReader);
            DynamicTensor result = odeFromTxt.Evaluate(odeFromTxt.GetTimeIn(), odeFromTxt.GetCondIn());
            std::cout << "ODE evaluated successfully at t0 and y0." << std::endl;
            std::cout << "Result: ";
            std::cout << result << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error evaluating ODE at t0 and y0: " << e.what() << std::endl;
        }
    
    std::cout << "ODE construction tests completed." << std::endl;
    std::cout << "==============================" << std::endl;

    std::cout << "==============================" << std::endl;
    std::cout << "Testing ODE direct constructors" << std::endl;

    try {
        // test scalar constructor
        std::unique_ptr<Function> f1 = std::make_unique<ParsedFunction>(
            std::vector<std::string>{"t + y0"}, std::vector<size_t>{1}
        );

        Ode scalarOde(0.5, 3.0, "scalar_ode", std::move(f1), nullptr);
        DynamicTensor result1 = scalarOde.Evaluate(scalarOde.GetTimeIn(), scalarOde.GetCondIn());
        std::cout << "Scalar ODE constructed and evaluated: "
                << result1 << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error testing scalar ODE constructor: " << e.what() << std::endl;
    }

    try {
        // test tensor constructor
        DynamicTensor y0vec(std::vector<double>{1.0, 2.0}, {2});
        std::unique_ptr<Function> f2 = std::make_unique<ParsedFunction>(
            std::vector<std::string>{"y0 + y1", "t + y0"}, std::vector<size_t>{2}
        );

        Ode vecOde(1.0, y0vec, "vector_ode", std::move(f2), nullptr);
        DynamicTensor result2 = vecOde.Evaluate(vecOde.GetTimeIn(), vecOde.GetCondIn());
        std::cout << "Vector ODE constructed and evaluated: "
                << result2 << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error testing vector ODE constructor: " << e.what() << std::endl;
    }

    std::cout << "==============================" << std::endl;
    std::cout << "Testing ODE RawData constructor" << std::endl;

    try {
        OdeRawData raw;
        raw.y0 = DynamicTensor(std::vector<double>{5.0}, {1});
        raw.time_params.t0 = 2.0;
        raw.function = std::make_unique<ParsedFunction>(
            std::vector<std::string>{"y0 * t"}, std::vector<size_t>{1}
        );

        Ode rawOde(raw);
        DynamicTensor result3 = rawOde.Evaluate(rawOde.GetTimeIn(), rawOde.GetCondIn());
        std::cout << "RawData ODE constructed and evaluated: "
                << result3 << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error testing RawData ODE constructor: " << e.what() << std::endl;
    }

    std::cout << "==============================" << std::endl;
    std::cout << "Testing ODE copy constructor & assignment" << std::endl;

    try {
        CsvReader csvReader("test_data.csv", ';', true);
        csvReader.Read();
        Ode original(csvReader);

        // copy constructor
        Ode copyConstructed(original);
        DynamicTensor r4 = copyConstructed.Evaluate(copyConstructed.GetTimeIn(),
                                                    copyConstructed.GetCondIn());
        std::cout << "Copy-constructed ODE evaluated: "
                << r4 << std::endl;

        // copy assignment
        Ode assigned = original;
        DynamicTensor r5 = assigned.Evaluate(assigned.GetTimeIn(), assigned.GetCondIn());
        std::cout << "Copy-assigned ODE evaluated: "
                << r5 << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error testing ODE copy operations: " << e.what() << std::endl;
    }

    std::cout << "All ODE constructor tests completed." << std::endl;
    std::cout << "==============================" << std::endl;

    return 0;
}
