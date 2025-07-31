#ifndef REGISTER_FILE_H
#define REGISTER_FILE_H
#include <cstdint>
#include <utility>
#include <iostream>

namespace sjtu {

class RegisterFile {
public:
  RegisterFile();
  std::pair<uint32_t, uint32_t> Result();
  void SetReadIndex1(const uint32_t &);
  void SetReadIndex2(const uint32_t &);
  void SetWriteIndex(const uint32_t &);
  void SetInput(const uint32_t &);
  void SetWriteEnable(const bool &);
  uint32_t GetReturnValue() const;
  void Print() const;
private:
  uint32_t register_[32];
  uint32_t read_index1_;
  uint32_t read_index2_;
  uint32_t write_index_;
  uint32_t input_;
  bool write_enable_;
};

}

#endif //REGISTER_FILE_H
