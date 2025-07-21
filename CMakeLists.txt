cmake_minimum_required(VERSION 3.22)
project(RISC_V_Simulator)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/include)

add_executable(RISC_V_Simulator
    src/selector.cpp
    src/arithmetic_logic_unit.cpp
    src/naive_main.cpp
)
