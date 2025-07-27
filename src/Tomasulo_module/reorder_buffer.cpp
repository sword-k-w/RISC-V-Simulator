#include "Tomasulo_module/reorder_buffer.h"

#include <Tomasulo_module/memory.h>
#include <Tomasulo_module/reservation_station.h>

namespace sjtu {

void ReorderBuffer::Run() {
  if (broadcast_dest_ != -1) {
    entry_[broadcast_dest_].ready = true;
    entry_[broadcast_dest_].value = broadcast_val_;
    broadcast_dest_ = -1;
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
