#include "Tomasulo_module/reorder_buffer.h"

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
    if (entry_[head_].instruction.format_type == B) {
      // TODO
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
  }

  mem_->las_rob_head_ = head_;
  mem_->las_rob_tail_ = tail_;
  rs_->las_rob_head_ = head_;
  rs_->las_rob_tail_ = tail_;
}

}
