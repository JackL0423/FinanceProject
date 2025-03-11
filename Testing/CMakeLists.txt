cmake_minimum_required(VERSION 3.27)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(ModelTests)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(${PROJECT_NAME} test_blackScholesModel.cpp test_optionGreeks.cpp test_optionGreeksModel.cpp test_hestonModel.cpp)

add_library(ScholesModel ../include/blackScholesModel.h ../src/blackScholesModel.cpp)

add_library(optionGreeks ../include/optionGreeks.h ../src/optionGreeks.cpp)

add_library(optionGreeksModel ../include/optionGreeksModel.h ../src/optionGreeksModel.cpp)

add_library(hestonModel ../include/hestonModel.h ../src/hestonModel.cpp)

target_link_libraries(${PROJECT_NAME} ScholesModel optionGreeks optionGreeksModel hestonModel GTest::gtest_main)

include(GoogleTest)

gtest_discover_tests(${PROJECT_NAME})

