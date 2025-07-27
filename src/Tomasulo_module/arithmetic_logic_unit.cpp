#include "Tomasulo_module/arithmetic_logic_unit.h"

#include <Tomasulo_module/reorder_buffer.h>

namespace sjtu {

ArithmeticLogicUnit::ArithmeticLogicUnit() : wireA_(0u), wireB_(0u), sel_(And) {}

void ArithmeticLogicUnit::Run() {
  if (dest_ != -1) {
    uint32_t res;
    switch (sel_) {
      case Add:
      case Addi:
      case Lb:
      case Lbu:
      case Lh:
      case Lhu:
      case Lw:
      case Sb:
      case Sh:
      case Sw:
      case Jal:
      case Jalr:
      case Auipc:
      case Lui:
        res = static_cast<uint32_t>(static_cast<int>(wireA_) + static_cast<int>(wireB_));
      break;
      case Sub:
        res = static_cast<uint32_t>(static_cast<int>(wireA_) - static_cast<int>(wireB_));
      break;
      case And:
      case Andi:
        res = wireA_ & wireB_;
      break;
      case Or:
      case Ori:
        res = wireA_ | wireB_;
      break;
      case Xor:
      case Xori:
        res = wireA_ ^ wireB_;
      break;
      case Sll:
      case Slli:
        res = wireA_ << wireB_;
      break;
      case Srl:
      case Srli:
        res = wireA_ >> wireB_;
      break;
      case Sra:
      case Srai:
        res = static_cast<uint32_t>(static_cast<int>(wireA_) >> wireB_);
      break;
      case Slt:
      case Slti:
      case Blt:
        res = static_cast<int>(wireA_) < static_cast<int>(wireB_) ? 1u : 0u;
      break;
      case Sltu:
      case Sltiu:
      case Bltu:
        res = wireA_ < wireB_ ? 1u : 0u;
      break;
      case Beq:
        res = wireA_ == wireB_ ? 1u : 0u;
      break;
      case Bge:
        res = static_cast<int>(wireA_) >= static_cast<int>(wireB_) ? 1u : 0u;
      break;
      case Bgeu:
        res = wireA_ >= wireB_ ? 1u : 0u;
      break;
      case Bne:
        res = wireA_ != wireB_ ? 1u : 0u;
      break;
      default:
        assert(0);
    }
    // TODO
    // send result to necessary modules
    rs_->broadcast_dest_ = dest_;
    rs_->broadcast_val_ = res;
    rob_->broadcast_dest_ = dest_;
    rob_->broadcast_val_ = res;
  }

}

}
