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

Memory::Memory() : instruction_parser_() {
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
  cur_instruction_ = instruction_parser_.Decode(pc_, GetInstruction());
}

void Memory::RunPC() {
  if ((las_rob_tail_ + 1) % 32 == las_rob_head_) {
    rob_->whether_new_instruction_ = false;
    rs_->whether_new_instruction_ = false;
    rf_->whether_dependence_ = false;
    return;
  }

  rob_->whether_new_instruction_ = true;
  rs_->whether_new_instruction_ = true;
  rob_->new_instruction_ = cur_instruction_;
  rs_->new_instruction_ = cur_instruction_;
  if (cur_instruction_.format_type == S || cur_instruction_.format_type == B) {
    rf_->whether_dependence_ = false;
  } else {
    rf_->whether_dependence_ = true;
    rf_->new_reg_id_ = cur_instruction_.rd;
    rf_->new_dependence_ = las_rob_tail_;
  }
  if (cur_instruction_.format_type == B) {
    // TODO
  } else if (cur_instruction_.format_type == J) {
    pc_ += cur_instruction_.immediate;
    cur_instruction_ = instruction_parser_.Decode(pc_, GetInstruction());
  } else if (cur_instruction_.format_type == IC) {
    // TODO
  } else {
    pc_ += 4;
    cur_instruction_ = instruction_parser_.Decode(pc_, GetInstruction());
    if (cur_instruction_.format_type == U) {
      cur_instruction_.immediate += pc_;
    }
  }
}

uint32_t Memory::GetInstruction() {
  return memory_[pc_] | (memory_[pc_ + 1] << 8) | (memory_[pc_ + 2] << 16) | (memory_[pc_ + 3] << 24);
}

}
