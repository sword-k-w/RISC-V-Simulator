#include "immediate_generator.h"

namespace sjtu {

ImmediateGenerator::ImmediateGenerator() : code_(0u), sel_(R) {}

uint32_t ImmediateGenerator::Result() const {
  switch (sel_) {
    case R:
      return 0u;
    case IA:
      return InstructionParser::Extract(code_, 20, 32);
    case IM:
      return InstructionParser::Extract(code_, 20, 32);
    case IC:
      return InstructionParser::Extract(code_, 20, 32);
    case Istar:
      return InstructionParser::Extract(code_, 20, 25);
    case S:
      return InstructionParser::Extract(code_, 25, 32) << 5 | InstructionParser::Extract(code_, 7, 12);
    case B: {
      uint32_t res = InstructionParser::Extract(code_, 31, 32) << 11;
      res |= InstructionParser::Extract(code_, 7, 8) << 10;
      res |= InstructionParser::Extract(code_, 25, 31) << 4;
      res |= InstructionParser::Extract(code_, 8, 12);
      return res;
    }
    case Uauipc:
      return InstructionParser::Extract(code_, 12, 32) << 12;
    case Ului:
      return InstructionParser::Extract(code_, 12, 32) << 12;
    case J: {
      uint32_t res = InstructionParser::Extract(code_, 7, 12);
      res = InstructionParser::Extract(code_, 31, 32) << 19;
      res |= InstructionParser::Extract(code_, 12, 20) << 11;
      res |= InstructionParser::Extract(code_, 20, 21) << 10;
      res |= InstructionParser::Extract(code_, 21, 31);
      return res;
    }
    default:
      assert(0);
  }
}

void ImmediateGenerator::SetCode(const uint32_t &input) {
  code_ = input;
}

void ImmediateGenerator::SetSel(const InstructionType &sel) {
  sel_ = sel;
}


}