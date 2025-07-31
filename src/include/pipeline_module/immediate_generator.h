#ifndef IMMEDIATE_GENERATOR_H
#define IMMEDIATE_GENERATOR_H
#include <cstdint>
#include "pipeline_module/instruction_parser.h"

namespace sjtu {

class ImmediateGenerator {
public:
  ImmediateGenerator();
  int32_t Result() const;
  void SetCode(const uint32_t &);
  void SetSel(const InstructionType &);
private:
  uint32_t code_;
  InstructionType sel_;
};

}

#endif //IMMEDIATE_GENERATOR_H
