#include "Tomasulo_module/arithmetic_logic_unit.h"

#include <Tomasulo_module/load_store_buffer.h>
#include <Tomasulo_module/reorder_buffer.h>

namespace sjtu {

void ArithmeticLogicUnit::Run() {
  rs_->alu_broadcast_dest_ = -1;
  rob_->alu_broadcast_dest_ = -1;
  lsb_->alu_broadcast_dest_ = -1;
  if (dest_ != -1 && !predict_failed_) {
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
    // if (is_zero_) {
    //   if (sel_ == Jalr) {
    //     wireS_ = 0;
    //   } else {
    //     res = 0;
    //   }
    // }
    if (sel_ != Lb && sel_ != Lbu && sel_ != Lh && sel_ != Lhu && sel_ != Lw && sel_ != Sb && sel_ != Sh && sel_ != Sw) {
      rs_->alu_broadcast_dest_ = dest_;
      if (sel_ == Jalr) {
        rs_->alu_broadcast_val_ = wireS_;
      } else {
        rs_->alu_broadcast_val_ = res;
      }
    }
    if (sel_ == Lb || sel_ == Lbu || sel_ == Lh || sel_ == Lhu || sel_ == Lw) {
      lsb_->alu_broadcast_dest_ = dest_;
      lsb_->alu_broadcast_address_ = res;
    } else {
      rob_->alu_broadcast_dest_ = dest_;
      if (sel_ == Sb || sel_ == Sh || sel_ == Sw || sel_ == Jalr) {
        rob_->alu_broadcast_address_ = res;
        rob_->alu_broadcast_val_ = wireS_;
      } else {
        rob_->alu_broadcast_val_ = res;
      }
    }
    dest_ = -1;
  }
}

void ArithmeticLogicUnit::Copy(const ArithmeticLogicUnit &other) {
  is_zero_ = other.is_zero_;
  predict_failed_ = other.predict_failed_;
  dest_ = other.dest_;
  wireA_ = other.wireA_;
  wireB_ = other.wireB_;
  wireS_ = other.wireS_;
  sel_ = other.sel_;
}

}
