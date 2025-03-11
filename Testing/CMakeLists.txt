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

# Enable testing
enable_testing()

# Add libraries
add_library(ScholesModel ../src/blackScholesModel.cpp)
add_library(optionGreeks ../src/optionGreeks.cpp)
add_library(optionGreeksModel ../src/optionGreeksModel.cpp)
add_library(hestonModel ../src/hestonModel.cpp)

# Set include directories for libraries
target_include_directories(ScholesModel PUBLIC ../include)
target_include_directories(optionGreeks PUBLIC ../include)
target_include_directories(optionGreeksModel PUBLIC ../include)
target_include_directories(hestonModel PUBLIC ../include)

# Set C++ standard for libraries
target_compile_features(ScholesModel PUBLIC cxx_std_23)
target_compile_features(optionGreeks PUBLIC cxx_std_23)
target_compile_features(optionGreeksModel PUBLIC cxx_std_23)
target_compile_features(hestonModel PUBLIC cxx_std_23)

# Add the test executable
add_executable(${PROJECT_NAME}
    test_blackScholesModel.cpp
    test_optionGreeks.cpp
    test_optionGreeksModel.cpp
    test_hestonModel.cpp
)

# Link libraries to the test executable
target_link_libraries(${PROJECT_NAME}
    ScholesModel
    optionGreeks
    optionGreeksModel
    hestonModel
    GTest::gtest_main
)

# Discover tests
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})

