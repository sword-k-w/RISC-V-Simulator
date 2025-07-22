#ifndef DATA_MEMORY_H
#define DATA_MEMORY_H

#include <cstdint>
#include <unordered_map>

namespace sjtu {

enum RWSize {
  Byte, Half, Word
};

class DataMemeory {
private:
  std::unordered_map<uint32_t, uint32_t> memory_;
  uint32_t address_;
  uint32_t input_;
  bool is_read_;
  bool is_unsigned_;
  RWSize type_;
public:
  DataMemeory();
  uint32_t Result();
  void SetAddress(const uint32_t &);
  void SetInput(const uint32_t &);
  void SetRW(const bool &);
  void SetUn(const bool &);
  void SetSize(const RWSize &);
};

}

#endif //DATA_MEMORY_H
