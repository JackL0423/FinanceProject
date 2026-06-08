cmake_minimum_required(VERSION 3.27)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Required for linking static libs into the pybind11 shared module
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

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
    target_include_directories(${LIB} PUBLIC ${CMAKE_SOURCE_DIR}/include)
endforeach()

# Add the test executable
add_executable(${PROJECT_NAME} src/main.cpp)

# Set include directories
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Link libraries to the executable
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

# Ensure inputReader.cpp and RMSE.cpp are included
target_sources(${PROJECT_NAME} PRIVATE src/inputReader.cpp src/RMSE.cpp)

# ----------------------------------------------------------------
# Python bridge (pybind11)
# ----------------------------------------------------------------
find_package(pybind11 QUIET)
if(pybind11_FOUND)
    set(PRICING_LIBS
        blackScholesModel
        optionGreeks
        optionGreeksModel
        hestonModel
        RMSE
    )

    pybind11_add_module(pricing_core src/bindings.cpp)
    target_include_directories(pricing_core PRIVATE ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(pricing_core PRIVATE ${PRICING_LIBS})
    target_compile_features(pricing_core PRIVATE cxx_std_23)

    message(STATUS "pybind11 found — pricing_core Python module will be built")
else()
    message(WARNING "pybind11 not found — skipping pricing_core Python module. "
                    "Install with: pip install pybind11 --break-system-packages  "
                    "or: brew install pybind11")
endif()