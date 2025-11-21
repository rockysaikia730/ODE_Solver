cmake_minimum_required(VERSION 3.22)
project(Codes)

set(CMAKE_CXX_STANDARD 20)

add_executable(ODEFunctionTest
        ODE.cpp
        ODEFunction.cpp
        VectorOutput.cpp
)

add_executable(ODETableTest
        ODE.cpp
        ODETable.cpp
        VectorOutput.cpp
)
