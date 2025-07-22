#include "instruction_memory.h"

namespace sjtu {

uint32_t HexDecoder(char c) {
  if (c >= '0' && c <= '9') {
    return c - '0';
  }
  return c - 'A' + 10;
}

InstructionMemory::InstructionMemory() {
  std::string tmp;
  uint32_t cur_address = 0u;
  uint32_t res;
  while (std::cin >> tmp) {
    if (tmp[0] == '@') {
      res = 0;
      for (int i = 8; i >= 1; --i) {
        res = res * 10 + HexDecoder(tmp[i]);
      }
      cur_address = res;
    } else {
      uint32_t base = 1u;
      res = HexDecoder(tmp[1]) * 16 + HexDecoder(tmp[0]);
      for (int i = 1; i <= 3; ++i) {
        base *= 256u;
        std::cin >> tmp;
        res += (HexDecoder(tmp[1]) * 16 + HexDecoder(tmp[0])) *base;
      }
      code_[cur_address / 4] = res;
      cur_address += 4;
    }
  }
}

uint32_t InstructionMemory::Result() const {
  assert(pc_ < kMaxN);
  return code_[pc_];
}

void InstructionMemory::SetPC(const uint32_t &input) {
  pc_ = input;
}

}