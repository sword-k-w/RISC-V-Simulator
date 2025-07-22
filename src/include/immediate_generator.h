#ifndef IMMEDIATE_GENERATOR_H
#define IMMEDIATE_GENERATOR_H
#include <cstdint>
#include "instruction_parser.h"

namespace sjtu {

class ImmediateGenerator {
private:
  uint32_t code_;
  InstructionType sel_;
public:
  ImmediateGenerator();
  uint32_t Result() const;
  void SetCode(const uint32_t &);
  void SetSel(const InstructionType &);
};

}

#endif //IMMEDIATE_GENERATOR_H
