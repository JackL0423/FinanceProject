cmake_minimum_required(VERSION 3.27)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project name
project(buildTest)

# Define library sources
set(LIBRARIES
    inputReader
    RMSE
    blackScholesModel
    optionGreeks
    optionGreeksModel
    hestonModel
)

# Add libraries
foreach(LIB ${LIBRARIES})
    add_library(${LIB} src/${LIB}.cpp)
    target_compile_features(${LIB} PUBLIC cxx_std_23)
endforeach()

# Add the test executable
add_executable(${PROJECT_NAME} src/main.cpp)

# Set include directories
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Link libraries to the executable
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

# Ensure inputReader.cpp and RMSE.cpp are included
target_sources(${PROJECT_NAME} PRIVATE src/inputReader.cpp src/RMSE.cpp)