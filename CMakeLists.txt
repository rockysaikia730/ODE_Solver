cmake_minimum_required(VERSION 3.22)
project(Codes)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexec-charset=UTF-8 -finput-charset=UTF-8")

# MuparserX library
file(GLOB MUPARSERX_SOURCES
    ${CMAKE_SOURCE_DIR}/external/muparserx/parser/*.cpp
)
list(FILTER MUPARSERX_SOURCES EXCLUDE REGEX "mpTest.cpp")
add_library(muparserx ${MUPARSERX_SOURCES})
target_include_directories(muparserx PUBLIC
    ${CMAKE_SOURCE_DIR}/external/muparserx/parser
)

# Google Test
# cmake_policy(SET CMP0135 NEW)
include(FetchContent)
FetchContent_Declare(
    googletest
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/googletest-03597a01ee50ed33e9dfd640b249b4be3799d395
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
include(GoogleTest)

# Everything executable
set(CORE_SOURCES
    src/dynamic_tensor.cpp
    src/ode.cpp                  
    src/ode_solver.cpp           
    src/ode_raw_data.cpp
    src/parsed_function.cpp
    src/function.cpp
    src/reader.cpp
    src/reader_csv.cpp
    src/reader_txt.cpp
    src/newton_raphson.cpp
    src/runge_kutta_solver.cpp  
    src/multistep_ode_solver.cpp
    src/adamsbashforth_solver.cpp
    src/implicit_solver.cpp
    src/backward_differentiation_solver.cpp
    src/adam_moulton_solver.cpp
    src/backward_euler_solver.cpp
    src/forward_euler_solver.cpp
    src/output.cpp
    src/output_plotter.cpp
)

# 2. Define a static library containing the logic
add_library(core_lib STATIC ${CORE_SOURCES})

# 3. Setup include directories and dependencies for the library
target_include_directories(core_lib PUBLIC
    include
    src
    ${CMAKE_SOURCE_DIR}/external/muparserx/parser
)

target_link_libraries(core_lib PUBLIC muparserx)

# --- TARGET: The Main Application ---

# 4. The main executable only contains main.cpp and links to the core library
add_executable(everything src/main.cpp)
target_link_libraries(everything PRIVATE core_lib)


# --- TARGET: The Unit Tests ---

add_executable(unittests
    Tests/dynamic_tensor_unittest.cpp
    Tests/function_unittest.cpp
    Tests/newton_raphson_unittest.cpp
    Tests/runge_kutta_unittest.cpp
    Tests/adamsbashforth_unittest.cpp
    Tests/backward_differentiation_unittest.cpp
    Tests/adam_moulton_unittest.cpp
    Tests/backward_euler_unittest.cpp
    Tests/forward_euler_unittest.cpp
    Tests/output_plotter.cpp
)

# 5. Link the unittests to the core library AND GTest
target_link_libraries(unittests
    PRIVATE
    core_lib
    GTest::gtest_main
)

gtest_discover_tests(unittests)


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

add_executable(output_unittest
    Tests/output_unittest.cpp
    src/output.cpp
    src/output_csv.cpp
    src/output_txt.cpp

    src/reader.cpp
    src/reader_csv.cpp
    src/reader_txt.cpp

    src/dynamic_tensor.cpp
    src/parsed_function.cpp
    src/function.cpp
    src/ode.cpp
    src/ode_raw_data.cpp
    src/ode_solver.cpp
    src/runge_kutta_solver.cpp
)

target_include_directories(output_unittest PRIVATE
    include
    src
    ${CMAKE_SOURCE_DIR}/external/muparserx/parser
    ${gtest_SOURCE_DIR}/include      
    ${gmock_SOURCE_DIR}/include     
)

target_link_libraries(output_unittest PRIVATE
    muparserx
    GTest::gtest_main
)

gtest_discover_tests(output_unittest)

#Generate documentation with Doxygen
# find_package(Doxygen)
# if(DOXYGEN_FOUND)
#    add_custom_target(doxygen ALL
#        COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
#        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs
#    )
# endif()
