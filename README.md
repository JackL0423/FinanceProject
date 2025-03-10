# FinanceProject

## Overview
FinanceProject is a financial modeling application that includes implementations of the Black-Scholes model and option Greeks calculations. The project is designed to provide tools for pricing European options and calculating the sensitivities of these options to various factors.

## Project Structure

## Key Components

### Black-Scholes Model

- Implemented in [`blackScholesModel`](include/blackScholesModel.h) and [`blackScholesModel.cpp`](src/blackScholesModel.cpp).
- Provides methods to calculate option prices and related parameters.

### Option Greeks

- Implemented in [`optionGreeks`](include/optionGreeks.h) and [`optionGreeks.cpp`](src/optionGreeks.cpp).
- Provides methods to calculate Delta, Gamma, Vega, Theta, and Rho.

### Unit Tests

- Implemented using Google Test framework.
- Tests for [`blackScholesModel`](include/blackScholesModel.h) are in [`test_blackScholesModel.cpp`](Testing/test_blackScholesModel.cpp).
- Tests for [`optionGreeks`](include/optionGreeks.h) are in [`test_optionGreeks.cpp`](Testing/test_optionGreeks.cpp).

## Build System

### CMake

- The project uses CMake to manage the build process.
- The main CMake configuration is in [`CMakeLists.txt`](CMakeLists.txt).
- The test configuration is in [`Testing/CMakeLists.txt`](Testing/CMakeLists.txt).

## Usage

### Building the Project

1. Navigate to the project directory:
    ```sh
    cd FinanceProject
    ```

2. Create a build directory and navigate into it:
    ```sh
    mkdir build
    cd build
    ```

3. Run CMake to generate the build system:
    ```sh
    cmake ..
    ```

4. Use `make` to compile the project:
    ```sh
    make
    ```

### Running Tests

1. Navigate to the [Testing](http://_vscodecontentref_/16) directory:
    ```sh
    cd make
    FinanceProject/Testing
    ```

2. Create a build directory and navigate into it:
    ```sh
    mkdir build
    cd build
    ```

3. Run CMake to generate the build system for tests:
    ```sh
    cmake ..
    ```

4. Use `make` to compile the tests:
    ```sh
    make
    ```

5. Run the tests:
    ```sh
    ./ModelTests
    ```
