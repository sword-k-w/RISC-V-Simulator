#include "Tomasulo_module/memory.h"
#include <string>
#include <iostream>
#include <Tomasulo_module/register_file.h>
#include <Tomasulo_module/reorder_buffer.h>
#include <Tomasulo_module/reservation_station.h>

namespace sjtu {

uint32_t HexDecoder(char c) {
  if (c >= '0' && c <= '9') {
    return c - '0';
  }
  return c - 'A' + 10;
}

Memory::Memory() {}

void Memory::Init() {
  std::string tmp;
  uint32_t cur_address = 0u;
  while (std::cin >> tmp) {
    if (tmp[0] == '@') {
      cur_address = 0;
      for (int i = 1; i <= 8; ++i) {
        cur_address = cur_address * 16 + HexDecoder(tmp[i]);
      }
    } else {
      memory_[cur_address] = HexDecoder(tmp[0]) * 16 + HexDecoder(tmp[1]);
      ++cur_address;
    }
  }
}


void Memory::RunPC() {
  if (thaw_) {
    assert(!predict_failed_);
    pc_ = new_pc_;
    jalr_frozen_ = false;
    thaw_ = false;
  }
  if (predict_failed_) {
    pc_ = new_pc_;
    jalr_frozen_ = false;
    predict_failed_ = false;
  }

  if ((las_rob_tail_ + 1) % 32 == las_rob_head_ || jalr_frozen_) {
    rob_->whether_new_instruction_ = false;
    rs_->whether_new_instruction_ = false;
    rf_->whether_dependence_ = false;
    return;
  }

  Instruction cur_instruction = instruction_parser_.Decode(pc_, GetInstruction());

  rob_->whether_new_instruction_ = true;
  rs_->whether_new_instruction_ = true;
  if (cur_instruction.format_type == S || cur_instruction.format_type == B) {
    rf_->whether_dependence_ = false;
  } else {
    rf_->whether_dependence_ = true;
    rf_->new_reg_id_ = cur_instruction.rd;
    rf_->new_dependence_ = las_rob_tail_;
  }
  if (cur_instruction.format_type == B) {
    bool predict = predictor_->Predict();
    uint32_t tmp = pc_;
    if (predict) {
      pc_ += cur_instruction.immediate;
      tmp += 4;
    } else {
      pc_ += 4;
      tmp += cur_instruction.immediate;
    }
    cur_instruction.predict = predict;
    cur_instruction.immediate = tmp;
  } else if (cur_instruction.format_type == J) {
    cur_instruction.immediate = pc_ + 4;
    pc_ += cur_instruction.immediate;
  } else if (cur_instruction.format_type == IC) {
    cur_instruction.immediate = pc_ + 4;
    jalr_frozen_ = true;
  } else {
    pc_ += 4;
    if (cur_instruction.type == Auipc) {
      cur_instruction.immediate += pc_;
    }
  }
  rob_->new_instruction_ = cur_instruction;
  rs_->new_instruction_ = cur_instruction;
}

void Memory::RunMemory() {
  if (whether_commit_) {
    if (commit_type_ == Sb || commit_type_ == Sh || commit_type_ == Sw) {
      switch (commit_type_) {
        case Sw:
          memory_[commit_address_ + 2] = commit_value_ >> 16 & 255;
          memory_[commit_address_ + 3] = commit_value_ >> 24 & 255;
        case Sh:
          memory_[commit_address_ + 1] = commit_value_ >> 8 & 255;
        case Sb:
          memory_[commit_address_] = commit_value_ & 255;
        default:
          assert(0);
      }
    } else {
      rob_->lsb_broadcast_dest_ = commit_dest_;
      switch (commit_type_) {
        case Lw:
          rob_->lsb_broadcast_val_ = (memory_[commit_address_] | (memory_[commit_address_ + 1] << 8)
            | (memory_[commit_address_ + 2] << 16) | (memory_[commit_address_ + 3] << 24));
          break;
        case Lhu:
          rob_->lsb_broadcast_val_ = memory_[commit_address_] | (memory_[commit_address_ + 1] << 8);
          break;
        case Lh:
          rob_->lsb_broadcast_val_ = static_cast<int>(memory_[commit_address_] | (memory_[commit_address_ + 1] << 8));
          break;
        case Lbu:
          rob_->lsb_broadcast_val_ = memory_[commit_address_];
          break;
        case Lb:
          rob_->lsb_broadcast_val_ = static_cast<int>(memory_[commit_address_]);
          break;
        default:
          assert(0);
      }
    }
    whether_commit_ = false;
  }
}

uint32_t Memory::GetInstruction() {
  return memory_[pc_] | (memory_[pc_ + 1] << 8) | (memory_[pc_ + 2] << 16) | (memory_[pc_ + 3] << 24);
}

void Memory::Copy(const Memory &other) {
  thaw_ = other.thaw_;
  predict_failed_ = other.predict_failed_;
  new_pc_ = other.new_pc_;
  new_type_ = other.new_type_;
  new_address_ = other.new_address_;
  new_value_ = other.new_value_;
  las_rob_head_ = other.las_rob_head_;
  las_rob_tail_ = other.las_rob_tail_;
  whether_commit_ = other.whether_commit_;
  commit_type_ = other.commit_type_;
  commit_address_ = other.commit_address_;
  commit_value_ = other.commit_value_;
  commit_dest_ = other.commit_dest_;
}

}
