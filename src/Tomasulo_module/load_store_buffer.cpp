#include "Tomasulo_module/load_store_buffer.h"

#include <Tomasulo_module/memory.h>

namespace sjtu {

void LoadStoreBuffer::PassInstruction() {
  if (waiting_cycle_ > 0) {
    --waiting_cycle_;
  } else if (waiting_cycle_ == 0) {
    mem_->whether_commit_ = true;
    mem_->commit_type_ = entry[head_].instruction.type;
    mem_->commit_address_ = entry[head_].address;
    if (entry[head_].instruction.format_type == S) {
      mem_->commit_value_ = entry[head_].value;
    } else {
      mem_->commit_dest_ = entry[head_].dest;
    }
    waiting_cycle_ = -1;
    head_ = (head_ + 1) % 32;
  } else if (head_ != tail_) {
    if (entry[head_].ready) {
      waiting_cycle_ = 1;
    }
  }
}

void LoadStoreBuffer::Run() {
  mem_->whether_commit_ = false;
  if (predict_failed_) {
    uint32_t nxt = (tail_ + 31) % 32;
    while (head_ != tail_ && (!entry[nxt].ready || entry[nxt].instruction.format_type == IM)) {
      tail_ = nxt;
      nxt = (tail_ + 31) % 32;
    }
    // if (tail_ != ((reset_tail_ + 1) % 32 == head_ ? head_ : reset_tail_)) {
    //   std::cerr << head_ << " " << tail_ << " " << reset_tail_ <<'\n';
    //   exit(0);
    // }
    // tail_ = (reset_tail_ + 1) % 32 == head_ ? head_ : reset_tail_;
    if (head_ == tail_) {
      waiting_cycle_ = -1;
    } else {
      PassInstruction();
    }
    mem_->las_lsb_head_ = head_;
    mem_->las_lsb_tail_ = tail_;
    rob_->las_lsb_tail_ = tail_;
    return;
  }

  if (alu_broadcast_dest_ != -1) {
    for (int i = head_; i != tail_; i = (i + 1) % 32) {
      if (!entry[i].ready && entry[i].dest == alu_broadcast_dest_) {
        entry[i].ready = true;
        entry[i].address = alu_broadcast_address_;
        break;
      }
    }
  }

  if (rob_broadcast_dest_ != -1) {
    for (int i = head_; i != tail_; i = (i + 1) % 32) {
      if (!entry[i].ready && entry[i].dest == rob_broadcast_dest_) {
        entry[i].ready = true;
        entry[i].address = rob_broadcast_address_;
        entry[i].value = rob_broadcast_value_;
      }
    }
  }

  if (whether_new_instruction_) {
    entry[tail_].instruction = new_instruction_;
    entry[tail_].ready = false;
    entry[tail_].dest = las_rob_tail_;
    tail_ = (tail_ + 1) % 32;
  }

  // for (int i = head_; i < tail_; ++i) {
  //   std::cerr << i << " ";
  //   entry[i].instruction.Print(std::cerr);
  //   std::cerr << '\n';
  // }

  PassInstruction();
  mem_->las_lsb_head_ = head_;
  mem_->las_lsb_tail_ = tail_;
  rob_->las_lsb_tail_ = tail_;
}

void LoadStoreBuffer::Copy(const LoadStoreBuffer &other) {
  predict_failed_ = other.predict_failed_;
  reset_tail_ = other.reset_tail_;
  las_rob_tail_ = other.las_rob_tail_;
  whether_new_instruction_ = other.whether_new_instruction_;
  new_instruction_ = other.new_instruction_;
  alu_broadcast_dest_ = other.alu_broadcast_dest_;
  alu_broadcast_address_ = other.alu_broadcast_address_;
  rob_broadcast_dest_ = other.rob_broadcast_dest_;
  rob_broadcast_address_ = other.rob_broadcast_address_;
  rob_broadcast_value_ = other.rob_broadcast_value_;
}

}
