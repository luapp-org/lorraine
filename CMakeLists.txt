# Lorraine Compiler

cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17) # C++ 17 must be installed

set(SRC_DIR "src")

project(Lorraine)

# Locate LLVM package
find_package(LLVM CONFIG REQUIRED)

# Include LLVM includes and definitions
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Create compiler executable
add_executable(compiler
            src/utils/utils.cpp
            src/compiler/compiler.cpp
            src/lexer/lexer.cpp
            src/cli/cli.cpp
            src/cli/config.cpp

            src/ast/expression.cpp
            src/ast/statement.cpp

            src/ast/type/type.cpp
            src/ast/type/descriptor/table.cpp
            src/ast/type/descriptor/function.cpp
            src/ast/type/descriptor/vararg.cpp
            src/ast/type/descriptor/array.cpp
            src/ast/type/descriptor/interface.cpp
            src/ast/type/validator.cpp

            src/parser/parser.cpp

            src/code_generation/code_generation.cpp
            src/code_generation/llvm_visitor.cpp
)

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(LLVM_LIBS support core irreader ${LLVM_TARGETS_TO_BUILD})

# Link against LLVM libraries
target_link_libraries(compiler ${LLVM_LIBS})