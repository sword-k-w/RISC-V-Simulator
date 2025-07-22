#ifndef INSTRUCTION_MEMORY_H
#define INSTRUCTION_MEMORY_H
#include <cstdint>
#include <cassert>
#include <string>
#include <iostream>

namespace sjtu {

class InstructionMemory {
private:
  static constexpr int kMaxN = 1e5 + 5;
  uint32_t pc_;
  uint32_t code_[kMaxN];
public:
  InstructionMemory();
  uint32_t Result() const;
  void SetPC(const uint32_t &);
};

}
#endif //INSTRUCTION_MEMORY_H
