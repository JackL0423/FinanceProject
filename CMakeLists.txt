
cmake_minimum_required(VERSION 3.27)

# cpp standard
set(CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# link libraries together... might fix errors in options.

# link project to main.cpp
project(buildTest VERSION 0.1
                  DESCRIPTION "Test to build project"
                  LANGUAGES CXX)


add_executable(${PROJECT_NAME} src/main.cpp)

