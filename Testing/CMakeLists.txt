cmake_minimum_required(VERSION 3.27)

project(ModelTests)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

# Ensure GoogleTest targets are available
find_package(GTest REQUIRED)

# Enable testing
enable_testing()

#TODO: #16 Fix makefile generation for gtest

set(LIBRARIES
    inputReader
    RMSE
    blackScholesModel
    optionGreeks
    optionGreeksModel
    hestonModel
)


# Add libraries
add_library(blackScholesModel ../src/blackScholesModel.cpp)
add_library(optionGreeks ../src/optionGreeks.cpp)
add_library(optionGreeksModel ../src/optionGreeksModel.cpp)
add_library(hestonModel ../src/hestonModel.cpp)
add_library(RMSE ../src/RMSE.cpp)
add_library(inputReader ../src/inputReader.cpp)

target_include_directories(blackScholesModel PUBLIC ../include)
target_include_directories(optionGreeks PUBLIC ../include)
target_include_directories(optionGreeksModel PUBLIC ../include)
target_include_directories(hestonModel PUBLIC ../include)
target_include_directories(RMSE PUBLIC ../include)
target_include_directories(inputReader PUBLIC ../include)
# Add test set cpp standard for compilation
target_compile_features(blackScholesModel PUBLIC cxx_std_23)
target_compile_features(optionGreeks PUBLIC cxx_std_23)
target_compile_features(optionGreeksModel PUBLIC cxx_std_23)
target_compile_features(hestonModel PUBLIC cxx_std_23)
target_compile_features(RMSE PUBLIC cxx_std_23)
target_compile_features(inputReader PUBLIC cxx_std_23)

# Add the test executable
add_executable(${PROJECT_NAME}
    test_blackScholesModel.cpp
    test_optionGreeks.cpp
    test_optionGreeksModel.cpp
    test_hestonModel.cpp
)

# Link libraries to the test executable
target_link_libraries(${PROJECT_NAME}
    blackScholesModel
    optionGreeks
    optionGreeksModel
    hestonModel
    RMSE 
    inputReader
    GTest::gtest_main
)

# Discover tests
include(googleTest)
gtest_discover_tests(${PROJECT_NAME})

