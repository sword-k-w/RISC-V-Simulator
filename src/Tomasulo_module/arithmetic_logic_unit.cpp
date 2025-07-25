#include "Tomasulo_module/arithmetic_logic_unit.h"

namespace sjtu {

ArithmeticLogicUnit::ArithmeticLogicUnit() : wireA_(0u), wireB_(0u), sel_(And) {}

void ArithmeticLogicUnit::Run() {
  uint32_t res;
  switch (sel_) {
    case Add:
      res = static_cast<uint32_t>(static_cast<int>(wireA_) + static_cast<int>(wireB_));
      break;
    case Sub:
      res = static_cast<uint32_t>(static_cast<int>(wireA_) - static_cast<int>(wireB_));
      break;
    case And:
      res = wireA_ & wireB_;
      break;
    case Or:
      res = wireA_ | wireB_;
      break;
    case Xor:
      res = wireA_ ^ wireB_;
      break;
    case Sll:
      res = wireA_ << wireB_;
      break;
    case Srl:
      res = wireA_ >> wireB_;
      break;
    case Sra:
      res = static_cast<uint32_t>(static_cast<int>(wireA_) >> wireB_);
      break;
    case Slt:
      res = static_cast<int>(wireA_) < static_cast<int>(wireB_) ? 1u : 0u;
      break;
    case Sltu:
      res = wireA_ < wireB_ ? 1u : 0u;
      break;
    default:
      assert(0);
  }
  // TODO
  // send result to necessary modules
}

}
