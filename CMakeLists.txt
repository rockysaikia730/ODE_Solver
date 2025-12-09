cmake_minimum_required(VERSION 3.22)
project(Codes)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Everything executable
add_executable(everything
    src/main.cpp
    src/dynamic_tensor.cpp
    src/ode.cpp
    src/ode_solver.cpp
    src/ode_raw_data.cpp
    src/parsed_function.cpp
    src/function.cpp
    src/reader.cpp
    src/reader_csv.cpp
    src/reader_txt.cpp
)

target_include_directories(everything PRIVATE
    include
    src
    ${CMAKE_SOURCE_DIR}/external/muparserx/parser
)

# MuparserX library
file(GLOB MUPARSERX_SOURCES
    ${CMAKE_SOURCE_DIR}/external/muparserx/parser/*.cpp
)
list(FILTER MUPARSERX_SOURCES EXCLUDE REGEX "mpTest.cpp")
add_library(muparserx ${MUPARSERX_SOURCES})
target_include_directories(muparserx PUBLIC
    ${CMAKE_SOURCE_DIR}/external/muparserx/parser
)
target_link_libraries(everything PRIVATE muparserx)

# Utilities library
add_library(utilities
    src/newton_raphson.cpp
    src/function.cpp
    src/dynamic_tensor.cpp
)
target_include_directories(utilities PUBLIC include)

# Google Test
cmake_policy(SET CMP0135 NEW)
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    GIT_PROGRESS TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(unittests
    tests/dynamic_tensor_unittest.cpp
    tests/function_unittest.cpp
    tests/newton_raphson_unittest.cpp
)
target_link_libraries(unittests
    utilities
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unittests)

# Legacy reader test (manual test)
add_executable(reader_test
    Tests/reader_test.cpp
    src/reader.cpp
    src/reader_csv.cpp
    src/reader_txt.cpp
    src/dynamic_tensor.cpp
    src/parsed_function.cpp
    src/function.cpp
    src/ode.cpp
    src/ode_solver.cpp
    src/ode_raw_data.cpp
)

target_include_directories(reader_test PRIVATE
    include
    src
    ${CMAKE_SOURCE_DIR}/external/muparserx/parser
)

target_link_libraries(reader_test PRIVATE muparserx GTest::gtest_main)

# New GoogleTest reader unittest
add_executable(reader_unittest
    tests/reader_unittest.cpp
    src/reader.cpp
    src/reader_csv.cpp
    src/reader_txt.cpp
    src/dynamic_tensor.cpp
    src/parsed_function.cpp
    src/function.cpp
    src/ode.cpp
    src/ode_solver.cpp
    src/ode_raw_data.cpp
)

target_include_directories(reader_unittest PRIVATE
    include
    src
    ${CMAKE_SOURCE_DIR}/external/muparserx/parser
)

target_link_libraries(reader_unittest PRIVATE
    muparserx
    GTest::gtest_main
)

gtest_discover_tests(reader_unittest)

# ODE unit tests
add_executable(ode_unittest
    tests/ode_unittest.cpp
    src/dynamic_tensor.cpp
    src/function.cpp
    src/parsed_function.cpp
    src/ode.cpp
    src/ode_solver.cpp
    src/ode_raw_data.cpp
    src/reader.cpp
    src/reader_csv.cpp
    src/reader_txt.cpp
)

target_include_directories(ode_unittest PRIVATE
    include
    src
    ${CMAKE_SOURCE_DIR}/external/muparserx/parser
)

target_link_libraries(ode_unittest PRIVATE
    muparserx
    GTest::gtest_main
)

gtest_discover_tests(ode_unittest)


# Generate documentation with Doxygen
#find_package(Doxygen)
#if(DOXYGEN_FOUND)
#    add_custom_target(doxygen ALL
#        COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
#        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs
#    )
#endif()
