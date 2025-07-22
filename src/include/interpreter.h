#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "selector.h"
#include "arithmetic_logic_unit.h"
#include "data_memory.h"
#include "register_file.h"
#include "instruction_parser.h"
#include "immediate_generator.h"
#include "branch_comparator.h"
#include "instruction_memory.h"

namespace sjtu {

class Interpreter {
private:
  uint32_t pc_;
  InstructionMemory IMEM_;
  RegisterFile RegFile_;
  ImmediateGenerator ImmGen_;
  BranchComparator BranchComp_;
  ArithmeticLogicUnit ALU_;
  DataMemeory DMEM_;
  InstructionParser parser_;
  Selector selectorA_;
  Selector selectorB_;
  Selector selectorPC_;
  Selector selectorWB0_;
  Selector selectorWB1_;
public:
  Interpreter();
  void Run();
};

}

#endif //INTERPRETER_H
