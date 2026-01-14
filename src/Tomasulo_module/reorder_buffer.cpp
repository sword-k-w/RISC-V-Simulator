#include "Tomasulo_module/reorder_buffer.h"

#include <Tomasulo_module/load_store_buffer.h>
#include <Tomasulo_module/memory.h>
#include <Tomasulo_module/register_file.h>
#include <Tomasulo_module/reservation_station.h>

namespace sjtu {

auto ReorderBuffer::Run() -> bool {
  alu_->predict_failed_ = false;
  other_->predict_failed = false;
  mem_->thaw_ = false;
  rs_->predict_failed_ = false;
  rf_->predict_failed_ = false;
  lsb_->predict_failed_ = false;
  mem_->predict_failed_ = false;
  rf_->whether_commit_ = false;
  lsb_->rob_broadcast_dest_ = -1;
  rf_->commit_rob_id_ = -1;

  if (predict_failed) {
    head_ = 0;
    tail_ = 0;
    mem_->las_rob_head_ = 0;
    mem_->las_rob_tail_ = 0;
    lsb_->las_rob_tail_ = 0;
    rs_->las_rob_tail_ = 0;
    rf_->new_dependence_ = 0;
    memcpy(rs_->old_rob_entry_, entry_, sizeof(entry_));
    return false;
  }

  if (alu_broadcast_dest_ != -1) {
    entry_[alu_broadcast_dest_].ready = true;
    entry_[alu_broadcast_dest_].value = alu_broadcast_val_;
    entry_[alu_broadcast_dest_].address = alu_broadcast_address_;
  }

  if (lsb_broadcast_dest_ != -1) {
    entry_[lsb_broadcast_dest_].ready = true;
    entry_[lsb_broadcast_dest_].value = lsb_broadcast_val_;
  }

  if (head_ != tail_ && entry_[head_].ready) {
    if (entry_[head_].instruction.type == Addi && entry_[head_].instruction.rd == 10
      && entry_[head_].instruction.immediate == 255 && entry_[head_].instruction.rs1 == 0) {
      return true;
    }
    if (entry_[head_].instruction.format_type == IC) {
      mem_->thaw_ = true;
      mem_->new_pc_ = entry_[head_].address;
      rf_->whether_commit_ = true;
      rf_->commit_rob_id_ = head_;
      rf_->commit_reg_id_ = entry_[head_].instruction.rd;
      rf_->commit_value_ = entry_[head_].value;
    } else if (entry_[head_].instruction.format_type == B) {
      predictor_->Feedback(entry_[head_].instruction.address_hash_val, entry_[head_].value, entry_[head_].instruction.predict);
      if (entry_[head_].value != (entry_[head_].instruction.predict & 1)) {
        alu_->predict_failed_ = true;
        rs_->predict_failed_ = true;
        rf_->predict_failed_ = true;
        lsb_->predict_failed_ = true;
        lsb_->reset_tail_ = entry_[head_].lsb_tail;
        mem_->predict_failed_ = true;
        other_->predict_failed = true;
        mem_->new_pc_ = entry_[head_].instruction.immediate;
        return false;
      }
    } else if (entry_[head_].instruction.format_type == S) {
      lsb_->rob_broadcast_address_ = entry_[head_].address;
      lsb_->rob_broadcast_value_ = entry_[head_].value;
      lsb_->rob_broadcast_dest_ = head_;
    } else {
      rf_->whether_commit_ = true;
      rf_->commit_rob_id_ = head_;
      rf_->commit_reg_id_ = entry_[head_].instruction.rd;
      rf_->commit_value_ = entry_[head_].value;
    }
    head_ = (head_ + 1) % 32;
  }

  if (whether_new_instruction_) {
    entry_[tail_].instruction = new_instruction_;
    entry_[tail_].ready = false;
    entry_[tail_].lsb_tail = las_lsb_tail_;
    tail_ = (tail_ + 1) % 32;
  }

  // std::cerr << "[RoB]\n";
  // for (int i = head_; i < tail_; ++i) {
  //   std::cerr << i << " ";
  //   entry_[i].instruction.Print(std::cerr);
  //   std::cerr << "       " << entry_[i].lsb_tail << '\n';
  // }

  mem_->las_rob_head_ = head_;
  mem_->las_rob_tail_ = tail_;
  lsb_->las_rob_tail_ = tail_;
  rs_->las_rob_tail_ = tail_;
  memcpy(rs_->old_rob_entry_, entry_, sizeof(entry_));
  rf_->new_dependence_ = tail_;
  return false;
}

void ReorderBuffer::Copy(const ReorderBuffer &other) {
  predict_failed = other.predict_failed;
  whether_new_instruction_ = other.whether_new_instruction_;
  new_instruction_ = other.new_instruction_;
  alu_broadcast_dest_ = other.alu_broadcast_dest_;
  alu_broadcast_val_ = other.alu_broadcast_val_;
  alu_broadcast_address_ = other.alu_broadcast_address_;
  lsb_broadcast_dest_ = other.lsb_broadcast_dest_;
  lsb_broadcast_val_ = other.lsb_broadcast_val_;
  las_lsb_tail_ = other.las_lsb_tail_;
}

}
