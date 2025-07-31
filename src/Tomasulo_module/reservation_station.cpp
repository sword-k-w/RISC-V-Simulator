#include "Tomasulo_module/reservation_station.h"

#include <Tomasulo_module/register_file.h>

namespace sjtu {

void ReservationStation::CheckDependence(uint32_t &val, int32_t &depend, const uint32_t &reg_id) {
  if (reg_id == 0 || old_dependence_[reg_id] == -1) {
    val = old_reg_[reg_id];
    depend = -1;
  } else if (old_rob_entry_[old_dependence_[reg_id]].ready) {
    val = old_rob_entry_[old_dependence_[reg_id]].value;
    depend = -1;
  } else {
    depend = old_dependence_[reg_id];
  }
}

void ReservationStation::Run() {
  alu_->dest_ = -1;
  alu_->is_zero_ = false;
  if (predict_failed_) {
    for (int32_t i = 0; i < 32; ++i) {
      entry_[i].busy = false;
      old_dependence_[i] = -1;
    }
    return;
  }

  if (whether_new_instruction_) {
    uint32_t index = 0;
    while (index < 32) {
      if (!entry_[index].busy) {
        break;
      }
      ++index;
    }
    if (index == 32) {
      assert(0);
    }
    entry_[index].is_zero = false;
    if (new_instruction_.rd == 0) {
      assert(new_instruction_.format_type == J || new_instruction_.format_type == IC || new_instruction_.format_type == S || new_instruction_.format_type == B);
      if (new_instruction_.format_type == J || new_instruction_.format_type == IC) {
        entry_[index].is_zero = true;
      }
    }
    entry_[index].busy = true;
    entry_[index].type = new_instruction_.type;
    entry_[index].dest = las_rob_tail_;
    if (new_instruction_.format_type == IC) {
      entry_[index].immediate_S = new_instruction_.rs2;
      entry_[index].depend2 = -1;
      entry_[index].val2 = new_instruction_.immediate;
      CheckDependence(entry_[index].val1, entry_[index].depend1, new_instruction_.rs1);
    } else if (new_instruction_.format_type == J || new_instruction_.format_type == U) {
      entry_[index].depend1 = -1;
      entry_[index].depend2 = -1;
      entry_[index].val1 = new_instruction_.immediate;
      entry_[index].val2 = 0;
    } else if (new_instruction_.format_type == S) {
      entry_[index].immediate_S = new_instruction_.immediate;
      CheckDependence(entry_[index].val1, entry_[index].depend1, new_instruction_.rs1);
      CheckDependence(entry_[index].val2, entry_[index].depend2, new_instruction_.rs2);
    } else if (new_instruction_.format_type != R && new_instruction_.format_type != B) {
      entry_[index].depend2 = -1;
      entry_[index].val2 = new_instruction_.immediate;
      CheckDependence(entry_[index].val1, entry_[index].depend1, new_instruction_.rs1);
    } else {
      CheckDependence(entry_[index].val1, entry_[index].depend1, new_instruction_.rs1);
      CheckDependence(entry_[index].val2, entry_[index].depend2, new_instruction_.rs2);
    }
  }

  if (alu_broadcast_dest_ != -1) {
    for (int32_t i = 0; i < 32; ++i) {
      if (entry_[i].depend1 == alu_broadcast_dest_) {
        entry_[i].depend1 = -1;
        entry_[i].val1 = alu_broadcast_val_;
      }
      if (entry_[i].depend2 == alu_broadcast_dest_) {
        entry_[i].depend2 = -1;
        entry_[i].val2 = alu_broadcast_val_;
      }
    }
  }

  if (lsb_broadcast_dest_ != -1) {
    for (int32_t i = 0; i < 32; ++i) {
      if (entry_[i].depend1 == lsb_broadcast_dest_) {
        entry_[i].depend1 = -1;
        entry_[i].val1 = lsb_broadcast_val_;
      }
      if (entry_[i].depend2 == lsb_broadcast_dest_) {
        entry_[i].depend2 = -1;
        entry_[i].val2 = lsb_broadcast_val_;
      }
    }
  }


  for (int32_t i = 0; i < 32; ++i) {
    if (entry_[i].busy && entry_[i].depend1 == -1 && entry_[i].depend2 == -1) {
      entry_[i].busy = false;
      if (entry_[i].type == Sb || entry_[i].type == Sh || entry_[i].type == Sw) {
        alu_->wireB_ = entry_[i].immediate_S;
        alu_->wireS_ = entry_[i].val2;
      } else {
        alu_->wireB_ = entry_[i].val2;
        if (entry_[i].type == Jalr) {
          alu_->wireS_ = entry_[i].immediate_S;
        }
      }
      alu_->is_zero_ = entry_[i].is_zero;
      alu_->wireA_ = entry_[i].val1;
      alu_->sel_ = entry_[i].type;
      alu_->dest_ = entry_[i].dest;
      break;
    }
  }
  // std::cerr << "<RS>\n";
  // for (int32_t i = 0; i < 32; ++i) {
  //   if (entry_[i].busy) {
  //     std::cerr << i << " ";
  //     switch (entry_[i].type) {
  //       case Add:
  //         std::cerr << "add";
  //       break;
  //       case Sub:
  //         std::cerr << "sub";
  //       break;
  //       case And:
  //         std::cerr << "and";
  //       break;
  //       case Or:
  //         std::cerr << "or";
  //       break;
  //       case Xor:
  //         std::cerr << "xor";
  //       break;
  //       case Sll:
  //         std::cerr << "sll";
  //       break;
  //       case Srl:
  //         std::cerr << "srl";
  //       break;
  //       case Sra:
  //         std::cerr << "sra";
  //       break;
  //       case Slt:
  //         std::cerr << "slt";
  //       break;
  //       case Sltu:
  //         std::cerr << "sltu";
  //       break;
  //       case Addi:
  //         std::cerr << "addi";
  //       break;
  //       case Andi:
  //         std::cerr << "andi";
  //       break;
  //       case Ori:
  //         std::cerr << "ori";
  //       break;
  //       case Xori:
  //         std::cerr << "xori";
  //       break;
  //       case Slli:
  //         std::cerr << "slli";
  //       break;
  //       case Srli:
  //         std::cerr << "srli";
  //       break;
  //       case Srai:
  //         std::cerr << "srai";
  //       break;
  //       case Slti:
  //         std::cerr << "slti";
  //       break;
  //       case Sltiu:
  //         std::cerr << "sltiu";
  //       break;
  //       case Lb:
  //         std::cerr << "lb";
  //       break;
  //       case Lbu:
  //         std::cerr << "lbu";
  //       break;
  //       case Lh:
  //         std::cerr << "lh";
  //       break;
  //       case Lhu:
  //         std::cerr << "lhu";
  //       break;
  //       case Lw:
  //         std::cerr << "lw";
  //       break;
  //       case Sb:
  //         std::cerr << "sb";
  //       break;
  //       case Sh:
  //         std::cerr << "sh";
  //       break;
  //       case Sw:
  //         std::cerr << "sw";
  //       break;
  //       case Beq:
  //         std::cerr << "beq";
  //       break;
  //       case Bge:
  //         std::cerr << "bge";
  //       break;
  //       case Bgeu:
  //         std::cerr << "bgeu";
  //       break;
  //       case Blt:
  //         std::cerr << "blt";
  //       break;
  //       case Bltu:
  //         std::cerr << "bltu";
  //       break;
  //       case Bne:
  //         std::cerr << "bne";
  //       break;
  //       case Jal:
  //         std::cerr << "jal";
  //       break;
  //       case Jalr:
  //         std::cerr << "jalr";
  //       break;
  //       case Auipc:
  //         std::cerr << "auipc";
  //       break;
  //       case Lui:
  //         std::cerr << "lui";
  //       break;
  //       default:
  //         assert(0);
  //     }
  //     std::cerr << " " << entry_[i].dest << " " << entry_[i].depend1 << " " << entry_[i].val1 << " " << entry_[i].depend2 << " " << entry_[i].val2 << '\n';
  //   }
  // }
  // std::cerr << '\n';
}

void ReservationStation::Copy(const ReservationStation &other) {
  predict_failed_ = other.predict_failed_;
  las_rob_tail_ = other.las_rob_tail_;
  whether_new_instruction_ = other.whether_new_instruction_;
  new_instruction_ = other.new_instruction_;
  alu_broadcast_dest_ = other.alu_broadcast_dest_;
  alu_broadcast_val_ = other.alu_broadcast_val_;
  lsb_broadcast_dest_ = other.lsb_broadcast_dest_;
  lsb_broadcast_val_ = other.lsb_broadcast_val_;
  memcpy(old_dependence_, other.old_dependence_, sizeof(old_dependence_));
  memcpy(old_reg_, other.old_reg_, sizeof(old_reg_));
  memcpy(old_rob_entry_, other.old_rob_entry_, sizeof(old_rob_entry_));
}

}
