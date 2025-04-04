# FinanceProject

## Overview

FinanceProject is a financial modeling application that includes implementations of the Black-Scholes model, Heston model, and option Greeks calculations. The project is designed to provide tools for pricing European options and calculating the sensitivities of these options to various factors.

## Project Structure

## Key Features

- **Black-Scholes Model**: Analytical pricing for European options.
- **Heston Model**: Stochastic volatility model with numerical integration and Monte Carlo simulation.
- **Option Greeks**: Sensitivities of option prices to various parameters.

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

## Error Handling

- Centralized error logging to `error.log` for all exceptions.
- Comprehensive exception handling to ensure robustness.

## Tests

- Unit tests implemented using Google Test framework.
- Covers basic functionality and edge cases for:
  - Black-Scholes model
  - Option Greeks
  - Heston model (including zero volatility, extreme correlation, and invalid inputs)

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

1. Navigate to the `Testing` directory:

    ```sh
    cd FinanceProject/Testing
    ```

2. Build and run the tests:

    ```sh
    mkdir build && cd build
    cmake ..
    make
    ./ModelTests
    ```

3. Check `error.log` for any runtime errors during testing.
