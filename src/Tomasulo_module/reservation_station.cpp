#include "Tomasulo_module/reservation_station.h"

#include <Tomasulo_module/register_file.h>

namespace sjtu {

void ReservationStation::Run() {
  if (predict_failed_) {
    for (int i = 0; i < 32; ++i) {
      entry_[i].busy = false;
    }
    predict_failed_ = false;
    return;
  }

  if (broadcast_dest_ != -1) {
    for (int i = 0; i < 32; ++i) {
      if (entry_[i].depend1 == broadcast_dest_) {
        entry_[i].val1 = broadcast_val_;
      }
      if (entry_[i].depend2 == broadcast_dest_) {
        entry_[i].val2 = broadcast_val_;
      }
    }
    broadcast_dest_ = -1;
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
    entry_[index].busy = true;
    entry_[index].type = new_instruction_.type;
    entry_[index].dest = las_rob_tail_;
    if (new_instruction_.format_type == J || new_instruction_.format_type == IC || new_instruction_.format_type == U) {
      entry_[index].depend1 = -1;
      entry_[index].depend2 = -1;
      entry_[index].val1 = new_instruction_.immediate;
      entry_[index].val2 = 0;
    } else if (new_instruction_.format_type == S) {
      entry_[index].immediate_S = new_instruction_.immediate;
      if (old_rf_->dependence_[new_instruction_.rs1] == -1) {
        entry_[index].depend1 = -1;
        entry_[index].val1 = old_rf_->reg_[new_instruction_.rs1];
      } else {
        entry_[index].depend1 = old_rf_->dependence_[new_instruction_.rs1];
      }
      if (old_rf_->dependence_[new_instruction_.rs2] == -1) {
        entry_[index].depend2 = -1;
        entry_[index].val2 = old_rf_->reg_[new_instruction_.rs2];
      } else {
        entry_[index].depend2 = old_rf_->dependence_[new_instruction_.rs2];
      }
    } else if (new_instruction_.format_type != R && new_instruction_.format_type != B) {
      entry_[index].depend2 = -1;
      entry_[index].val2 = new_instruction_.immediate;
      if (old_rf_->dependence_[new_instruction_.rs1] == -1) {
        entry_[index].depend1 = -1;
        entry_[index].val1 = old_rf_->reg_[new_instruction_.rs1];
      } else {
        entry_[index].depend1 = old_rf_->dependence_[new_instruction_.rs1];
      }
    } else {
      if (old_rf_->dependence_[new_instruction_.rs1] == -1) {
        entry_[index].depend1 = -1;
        entry_[index].val1 = old_rf_->reg_[new_instruction_.rs1];
      } else {
        entry_[index].depend1 = old_rf_->dependence_[new_instruction_.rs1];
      }
      if (old_rf_->dependence_[new_instruction_.rs2] == -1) {
        entry_[index].depend2 = -1;
        entry_[index].val2 = old_rf_->reg_[new_instruction_.rs2];
      } else {
        entry_[index].depend2 = old_rf_->dependence_[new_instruction_.rs2];
      }
    }
  }
  bool flag = true;
  for (int i = 0; i < 32; ++i) {
    if (entry_[i].busy && entry_[i].depend1 == -1 && entry_[i].depend2 == -1) {
      entry_[i].busy = false;
      if (entry_[i].type == Sb || entry_[i].type == Sh || entry_[i].type == Sw) {
        alu_->wireB_ = entry_[i].immediate_S;
        alu_->wireS_ = entry_[i].val2;
      } else {
        alu_->wireB_ = entry_[i].val2;
      }
      alu_->wireA_ = entry_[i].val1;
      alu_->sel_ = entry_[i].type;
      alu_->dest_ = entry_[i].dest;
      flag = false;
      break;
    }
  }
  if (flag) {
    alu_->dest_ = -1;
  }
}

void ReservationStation::Copy(const ReservationStation &other) {
  predict_failed_ = other.predict_failed_;
  las_rob_tail_ = other.las_rob_tail_;
  whether_new_instruction_ = other.whether_new_instruction_;
  new_instruction_ = other.new_instruction_;
}

}
