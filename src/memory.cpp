#include "memory.h"

namespace sjtu {

uint32_t HexDecoder(char c) {
  if (c >= '0' && c <= '9') {
    return c - '0';
  }
  return c - 'A' + 10;
}

Memory::Memory() : address_(0u), input_(0u), is_read_(true), pc_(0u), is_unsigned_(true), type_(Byte) {
  std::string tmp;
  uint32_t cur_address = 0u;
  while (std::cin >> tmp) {
    if (tmp[0] == '@') {
      cur_address = 0;
      for (int i = 1; i <= 8; ++i) {
        cur_address = cur_address * 16 + HexDecoder(tmp[i]);
      }
    } else {
      memory_[cur_address] = HexDecoder(tmp[0]) * 16 + HexDecoder(tmp[1]);
      ++cur_address;
    }
  }
}

uint32_t Memory::DataResult() {
  if (is_read_) {
    switch (type_) {
      case Word:
        return memory_[address_] | (memory_[address_ + 1] << 8) | (memory_[address_ + 2] << 16) | (memory_[address_ + 3] << 24) ;
      case Half:
        if (is_unsigned_) {
          return memory_[address_] | (memory_[address_ + 1] << 8);
        }
        return static_cast<int>(memory_[address_] | (memory_[address_ + 1] << 8));
      case Byte:
        if (is_unsigned_) {
          return memory_[address_];
        }
        return static_cast<int>(memory_[address_]);
    }
  }
  switch (type_) {
    case Byte:
      memory_[address_] = input_ & 255;
    case Half:
      memory_[address_ + 1] = input_ >> 8 & 255;
    case Word:
      memory_[address_ + 2] = input_ >> 16 & 255;
      memory_[address_ + 3] = input_ >> 24 & 255;
  }
  return input_;
}

void Memory::SetAddress(const uint32_t &input) {
  address_ = input;
}

void Memory::SetInput(const uint32_t &input) {
  input_ = input;
}

void Memory::SetRW(const bool &is_read) {
  is_read_ = is_read;
}

void Memory::SetUn(const bool &is_unsigned) {
  is_unsigned_ = is_unsigned;
}

void Memory::SetSize(const RWSize &type) {
  type_ = type;
}

uint32_t Memory::InstructionResult() {
  return memory_[pc_] | (memory_[pc_ + 1] << 8) | (memory_[pc_ + 2] << 16) | (memory_[pc_ + 3] << 24);
}

void Memory::SetPC(const uint32_t &input) {
  pc_ = input;
}

}