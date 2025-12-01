cmake_minimum_required(VERSION 3.22)
project(Codes)

set(CMAKE_CXX_STANDARD 20)

add_executable(everything
        src/main.cpp
        src/dynamic_tensor.cpp
		src/ode.cpp
		src/ode_solver.cpp
)

target_include_directories(everything PRIVATE include)

find_package(Doxygen)
if(DOXYGEN_FOUND)
	add_custom_target(doxygen ALL

	COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs
	COMMENT "Generating Doxygen Documentation"
	VERBATIM)
endif()
