#include "data_memory.h"

namespace sjtu {

DataMemeory::DataMemeory() : address_(0u), input_(0u), is_read_(true) {}

uint32_t DataMemeory::Result() {
  if (is_read_) {
    switch (type_) {
      case Word:
        return memory_[address_];
      case Half:
        if (is_unsigned_) {
          return memory_[address_] & ((1u << 16) - 1);
        }
        return static_cast<int>(memory_[address_]) & ((1u << 16) - 1);
      case Byte:
        if (is_unsigned_) {
          return memory_[address_] & ((1u << 8) - 1);
        }
        return static_cast<int>(memory_[address_]) & ((1u << 8) - 1);
    }
  }
  switch (type_) {
    case Word:
      return memory_[address_] = input_;
    case Half:
      return memory_[address_] = input_ & ((1u << 16) - 1);
    case Byte:
      return memory_[address_] = input_ & ((1u << 8) - 1);
  }
}

void DataMemeory::SetAddress(const uint32_t &input) {
  address_ = input;
}

void DataMemeory::SetInput(const uint32_t &input) {
  input_ = input;
}

void DataMemeory::SetRW(const bool &is_read) {
  is_read_ = is_read;
}

void DataMemeory::SetUn(const bool &is_unsigned) {
  is_unsigned_ = is_unsigned;
}

void DataMemeory::SetSize(const RWSize &type) {
  type_ = type;
}



}