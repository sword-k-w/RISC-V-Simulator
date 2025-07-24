#include "pipeline_module/arithmetic_logic_unit.h"

namespace sjtu {

ArithmeticLogicUnit::ArithmeticLogicUnit() : wireA_(0u), wireB_(0u), sel_(And) {}

uint32_t ArithmeticLogicUnit::Result() const {
  switch (sel_) {
    case Add:
      return static_cast<uint32_t>(static_cast<int>(wireA_) + static_cast<int>(wireB_));
    case Sub:
      return static_cast<uint32_t>(static_cast<int>(wireA_) - static_cast<int>(wireB_));
    case And:
      return wireA_ & wireB_;
    case Or:
      return wireA_ | wireB_;
    case Xor:
      return wireA_ ^ wireB_;
    case Sll:
      return wireA_ << wireB_;
    case Srl:
      return wireA_ >> wireB_;
    case Sra:
      return static_cast<uint32_t>(static_cast<int>(wireA_) >> wireB_);
    case Slt:
      return static_cast<int>(wireA_) < static_cast<int>(wireB_) ? 1u : 0u;
    case Sltu:
      return wireA_ < wireB_ ? 1u : 0u;
  }
  assert(0);
}

void ArithmeticLogicUnit::SetWireA(const uint32_t &input) {
  wireA_ = input;
}

void ArithmeticLogicUnit::SetWireB(const uint32_t &input) {
  wireB_ = input;
}

void ArithmeticLogicUnit::SetSel(const ArithmeticType &sel) {
  sel_ = sel;
}

}
