#include "register_file.h"

namespace sjtu {

RegisterFile::RegisterFile() : read_index1_(0u), read_index2_(0u), write_index_(0u), input_(0u),write_enable_(false) {
  for (int i = 0; i < 32; ++i) {
    register_[i] = 0u;
  }
}

std::pair<uint32_t, uint32_t> RegisterFile::Result() {
  if (write_enable_) {
    register_[write_index_] = input_;
  }
  return std::make_pair(register_[read_index1_], register_[read_index2_]);
}

void RegisterFile::SetReadIndex1(const uint32_t &input) {
  read_index1_ = input;
}

void RegisterFile::SetReadIndex2(const uint32_t &input) {
  read_index2_ = input;
}

void RegisterFile::SetWriteIndex(const uint32_t &input) {
  write_index_ = input;
}

void RegisterFile::SetInput(const uint32_t &input) {
  input_ = input;
}

void RegisterFile::SetWriteEnable(const bool &write_enable) {
  write_enable_ = write_enable;
}

uint32_t RegisterFile::GetReturnValue() const {
  return register_[10] & 255;
}

}