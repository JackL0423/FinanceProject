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

set(LIBRARIES
    inputReader
    RMSE
    blackScholesModel
    optionGreeks
    optionGreeksModel
    hestonModel
)

set(TEST_LIBRARIES
    test_blackScholesModel
    test_optionGreeks
    test_optionGreeksModel
    test_HestonModel
)

# Add libraries
foreach(LIB ${LIBRARIES})
    add_library(${LIB} ../src/${LIB}.cpp)
    target_compile_features(${LIB} PUBLIC cxx_std_23)
    target_include_directories(${LIB} PUBLIC ../include)
endforeach()

# Add test libraries
foreach(TEST_LIB ${TEST_LIBRARIES})
    add_library(${TEST_LIB} ${TEST_LIB}.cpp)
    target_compile_features(${TEST_LIB} PUBLIC cxx_std_23)
endforeach()

# Add the test executable
add_executable(${PROJECT_NAME}
    test_blackScholesModel.cpp
    test_optionGreeks.cpp
    test_optionGreeksModel.cpp
    test_hestonModel.cpp
)

# Link libraries to the test executable
target_link_libraries(${PROJECT_NAME}
    GTest::gtest
    ${LIBRARIES}
    ${TEST_LIBRARIES}
)

# Discover tests
include(googleTest)
gtest_discover_tests(${PROJECT_NAME})

