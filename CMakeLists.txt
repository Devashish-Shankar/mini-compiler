cmake_minimum_required(VERSION 4.0)

project(MiniCompiler
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler warnings
add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
)

# Project headers
include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

# ---------------------------------------------------------
# LLVM
# ---------------------------------------------------------

# Use the LLVM 21 installation provided by Ubuntu
set(LLVM_DIR "/usr/lib/llvm-21/lib/cmake/llvm")

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_VERSION}")
message(STATUS "LLVM include directory: ${LLVM_INCLUDE_DIRS}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# ---------------------------------------------------------
# Mini Compiler
# ---------------------------------------------------------

add_executable(minic
    src/main.cpp
    src/lexer/Lexer.cpp
    src/parser/Parser.cpp
    src/parser/AST.cpp
)
