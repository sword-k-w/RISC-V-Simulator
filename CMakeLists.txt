cmake_minimum_required(VERSION 3.22)
project(RISC_V_Simulator)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/include)

#add_definitions(-DDEBUG)

#add_executable(code
#    src/pipeline_module/selector.cpp
#    src/pipeline_module/arithmetic_logic_unit.cpp
#    src/pipeline_module/memory.cpp
#    src/pipeline_module/register_file.cpp
#    src/pipeline_module/instruction_parser.cpp
#    src/pipeline_module/branch_comparator.cpp
#    src/pipeline_module/immediate_generator.cpp
#    src/interpreter.cpp
#    src/naive_main.cpp
#)

add_executable(code
        src/Tomasulo_module/arithmetic_logic_unit.cpp
        src/Tomasulo_module/instruction_parser.cpp
        src/Tomasulo_module/reservation_station.cpp
        src/Tomasulo_module/register_file.cpp
        src/Tomasulo_module/reorder_buffer.cpp
        src/Tomasulo_module/load_store_buffer.cpp
        src/Tomasulo_module/memory.cpp
        src/simulator.cpp
        src/main.cpp
)