#include "Tomasulo_module/reorder_buffer.h"

#include <Tomasulo_module/load_store_buffer.h>
#include <Tomasulo_module/memory.h>
#include <Tomasulo_module/register_file.h>
#include <Tomasulo_module/reservation_station.h>

namespace sjtu {

void ReorderBuffer::Run() {
  if (broadcast_dest_ != -1) {
    entry_[broadcast_dest_].ready = true;
    entry_[broadcast_dest_].value = broadcast_val_;
    broadcast_dest_ = -1;
  }

  if (head_ < tail_ && entry_[head_].ready) {
    if (entry_[head_].instruction.type == Addi && entry_[head_].instruction.rd == 10
      && entry_[head_].instruction.immediate == 255 && entry_[head_].instruction.rs1 == 0) {
      std::cout << (rf_->reg_[10] & 255) << '\n';
      exit(0);
    }
    if (entry_[head_].instruction.format_type == B) {
      // TODO
      predictor_->Feedback(entry_[head_].value);
      if (entry_[head_].value != entry_[head_].instruction.predict) {
        rs_->predict_failed_ = true;
        rf_->predict_failed_ = true;
        lsb_->predict_failed_ = true;
        mem_->predict_failed_ = true;
        mem_->new_pc_ = entry_[head_].instruction.immediate;
        head_ = 0;
        tail_ = 0;
        mem_->las_rob_head_ = 0;
        mem_->las_rob_tail_ = 0;
        rs_->las_rob_head_ = 0;
        rs_->las_rob_tail_ = 0;
        return;
      }
    } else if (entry_[head_].instruction.format_type == S) {
      // TODO
    } else {
      rf_->whether_commit_ = true;
      rf_->commit_rob_id_ = head_;
      rf_->commit_reg_id_ = entry_[head_].instruction.rd;
      rf_->commit_value_ = entry_[head_].value;
    }
    ++head_;
  }

  if (whether_new_instruction_) {
    entry_[tail_].instruction = new_instruction_;
    entry_[tail_].ready = false;
    tail_ = (tail_ + 1) % 32;
    whether_new_instruction_ = false;
  }

  mem_->las_rob_head_ = head_;
  mem_->las_rob_tail_ = tail_;
  rs_->las_rob_head_ = head_;
  rs_->las_rob_tail_ = tail_;
}

}
