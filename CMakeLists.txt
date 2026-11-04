cmake_minimum_required(VERSION 3.16)
project(MatrixLibrary LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(app
    src/main.cpp
    src/matrix.cpp
)
target_include_directories(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_options(app PRIVATE
    -Wall
    -Wextra
)
include_directories(include)
add_executable(matrix_calculator src/main.cpp src/matrix.cpp)
enable_testing()
add_subdirectory(tests)
