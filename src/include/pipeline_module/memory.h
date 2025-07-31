#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <unordered_map>
#include <string>
#include <iostream>
#include <cassert>

namespace sjtu {

enum RWSize {
  Byte, Half, Word
};

class Memory {
public:
  Memory();
  uint32_t DataResult();
  void SetAddress(const uint32_t &);
  void SetInput(const uint32_t &);
  void SetRW(const bool &);
  void SetUn(const bool &);
  void SetSize(const RWSize &);

  uint32_t InstructionResult();
  void SetPC(const uint32_t &);
private:
  std::unordered_map<uint32_t, uint32_t> memory_;
  uint32_t pc_;
  uint32_t address_;
  uint32_t input_;
  bool is_read_;
  bool is_unsigned_;
  RWSize type_;
};

}

#endif //MEMORY_H
