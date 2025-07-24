#include "pipeline_module/selector.h"

namespace sjtu {

Selector::Selector() : wire0_(0u), wire1_(0u), sel_(false) {}

uint32_t Selector::Result() const {
  return sel_ ? wire1_ : wire0_;
}

void Selector::SetWire0(const uint32_t &input) {
  wire0_ = input;
}

void Selector::SetWire1(const uint32_t &input) {
  wire1_ = input;
}

void Selector::SetSel(const bool &sel) {
  sel_ = sel;
}

}