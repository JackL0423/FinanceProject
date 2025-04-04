cmake_minimum_required(VERSION 3.27)

# cpp standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# link libraries together... might fix errors in options.

# link project to main.cpp
project(buildTest)

add_library(ScholesModel src/blackScholesModel.cpp)
add_library(optionGreeks src/optionGreeks.cpp)
add_library(optionGreeksModel src/optionGreeksModel.cpp)
add_library(hestonModel src/hestonModel.cpp)

# Set include directories for libraries
target_include_directories(ScholesModel PUBLIC /include)
target_include_directories(optionGreeks PUBLIC /include)
target_include_directories(optionGreeksModel PUBLIC /include)
target_include_directories(hestonModel PUBLIC /include)

# Set C++ standard for libraries
target_compile_features(ScholesModel PUBLIC cxx_std_23)
target_compile_features(optionGreeks PUBLIC cxx_std_23)
target_compile_features(optionGreeksModel PUBLIC cxx_std_23)
target_compile_features(hestonModel PUBLIC cxx_std_23)

# Add the test executable
add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME}
    ScholesModel
    optionGreeks
    optionGreeksModel
    hestonModel
)