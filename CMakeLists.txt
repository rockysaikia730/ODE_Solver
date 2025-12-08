cmake_minimum_required(VERSION 3.22)
project(Codes)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
	GIT_PROGRESS TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# add_executable(everything
#         src/main.cpp
#         src/dynamic_tensor.cpp
# 		src/ode.cpp
# 		src/ode_solver.cpp
# )


add_library(utilities
	# src/newton_raphson.cpp
	src/runge_kutta_solver.cpp
	src/ode_solver.cpp
	src/ode.cpp
	src/function.cpp
    src/dynamic_tensor.cpp
)
target_include_directories(utilities PUBLIC include)

enable_testing()
add_executable(
  unittests
#   tests/dynamic_tensor_unittest.cpp
#   tests/function_unittest.cpp
#   tests/newton_raphson_unittest.cpp
	tests/runge_kutta_unittest.cpp
)

target_link_libraries(
	unittests
	utilities
	GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unittests)

# find_package(Doxygen)
# if(DOXYGEN_FOUND)
# 	add_custom_target(doxygen ALL

# 	COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
# 	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs
# 	COMMENT "Generating Doxygen Documentation"
# 	VERBATIM)
# endif()
