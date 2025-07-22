cmake_minimum_required(VERSION 3.22)
project(RISC_V_Simulator)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/include)

add_executable(code
    src/selector.cpp
    src/arithmetic_logic_unit.cpp
    src/data_memory.cpp
    src/register_file.cpp
    src/instruction_parser.cpp
    src/naive_main.cpp
)
