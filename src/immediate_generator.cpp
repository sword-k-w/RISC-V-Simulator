#include "immediate_generator.h"

namespace sjtu {

int ExtendSign(uint32_t x, int bit) {
  if (x >> bit & 1) {
    for (int i = bit + 1; i < 32; ++i) {
      x |= 1u << i;
    }
  }
  return x;
}

ImmediateGenerator::ImmediateGenerator() : code_(0u), sel_(R) {}

int ImmediateGenerator::Result() const {
  switch (sel_) {
    case R:
      return 0u;
    case IA:
      return ExtendSign(InstructionParser::Extract(code_, 20, 32), 11);
    case IM:
      return ExtendSign(InstructionParser::Extract(code_, 20, 32), 11);
    case IC:
      return ExtendSign(InstructionParser::Extract(code_, 20, 32), 11);
    case Istar:
      return InstructionParser::Extract(code_, 20, 25);
    case S:
      return ExtendSign(InstructionParser::Extract(code_, 25, 32) << 5 | InstructionParser::Extract(code_, 7, 12), 11);
    case B: {
      uint32_t res = InstructionParser::Extract(code_, 31, 32) << 12;
      res |= InstructionParser::Extract(code_, 7, 8) << 11;
      res |= InstructionParser::Extract(code_, 25, 31) << 5;
      res |= InstructionParser::Extract(code_, 8, 12) << 1;
      return ExtendSign(res, 12);
    }
    case Uauipc:
      return InstructionParser::Extract(code_, 12, 32) << 12;
    case Ului:
      return InstructionParser::Extract(code_, 12, 32) << 12;
    case J: {
      uint32_t res = InstructionParser::Extract(code_, 31, 32) << 20;
      res |= InstructionParser::Extract(code_, 12, 20) << 12;
      res |= InstructionParser::Extract(code_, 20, 21) << 11;
      res |= InstructionParser::Extract(code_, 21, 31) << 1;
      return ExtendSign(res, 20);
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