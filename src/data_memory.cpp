#include "data_memory.h"

namespace sjtu {

DataMemeory::DataMemeory() : address_(0u), input_(0u), is_read_(true) {}

uint32_t DataMemeory::Result() {
  if (is_read_) {
    return memory_[address_];
  }
  return memory_[address_] = input_;
}

void DataMemeory::SetAddress(const uint32_t &address) {
  address_ = address;
}

void DataMemeory::SetInput(const uint32_t &input) {
  input_ = input;
}

void DataMemeory::SetRW(const bool &is_read) {
  is_read_ = is_read;
}

}