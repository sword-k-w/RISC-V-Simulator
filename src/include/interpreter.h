#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "pipeline_module/selector.h"
#include "pipeline_module/arithmetic_logic_unit.h"
#include "pipeline_module/memory.h"
#include "pipeline_module/register_file.h"
#include "pipeline_module/instruction_parser.h"
#include "pipeline_module/immediate_generator.h"
#include "pipeline_module/branch_comparator.h"

namespace sjtu {

class Interpreter {
public:
  Interpreter();
  void Run();
private:
  uint32_t pc_;
  Memory MEM_;
  RegisterFile RegFile_;
  ImmediateGenerator ImmGen_;
  BranchComparator BranchComp_;
  ArithmeticLogicUnit ALU_;
  InstructionParser parser_;
  Selector selectorA_;
  Selector selectorB_;
  Selector selectorPC_;
  Selector selectorWB0_;
  Selector selectorWB1_;
};

}

#endif //INTERPRETER_H
