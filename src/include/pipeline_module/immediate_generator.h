#ifndef IMMEDIATE_GENERATOR_H
#define IMMEDIATE_GENERATOR_H
#include <cstdint>
#include "pipeline_module/instruction_parser.h"

namespace sjtu {

class ImmediateGenerator {
private:
  uint32_t code_;
  InstructionType sel_;
public:
  ImmediateGenerator();
  int32_t Result() const;
  void SetCode(const uint32_t &);
  void SetSel(const InstructionType &);
};

}

#endif //IMMEDIATE_GENERATOR_H
