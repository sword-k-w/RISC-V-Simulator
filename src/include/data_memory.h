#ifndef DATA_MEMORY_H
#define DATA_MEMORY_H

#include <cstdint>
#include <unordered_map>
#include <string>
#include <iostream>

namespace sjtu {

enum RWSize {
  Byte, Half, Word
};

class Memory {
private:
  std::unordered_map<uint32_t, uint32_t> memory_;
  uint32_t pc_;
  uint32_t address_;
  uint32_t input_;
  bool is_read_;
  bool is_unsigned_;
  RWSize type_;
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
};

}

#endif //DATA_MEMORY_H
