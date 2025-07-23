#include "instruction_parser.h"

namespace sjtu {

void Instruction::Print() const {
  switch (type) {
    case R:
      std::cerr << "(R) ";
      switch (funct) {
        case 0b0000000000:
          std::cerr << "add";
          break;
        case 0b0000100000:
          std::cerr << "sub";
          break;
        case 0b1110000000:
          std::cerr << "and";
          break;
        case 0b1100000000:
          std::cerr << "or";
          break;
        case 0b1000000000:
          std::cerr << "xor";
          break;
        case 0b0010000000:
          std::cerr << "sll";
          break;
        case 0b1010000000:
          std::cerr << "srl";
          break;
        case 0b1010100000:
          std::cerr << "sra";
          break;
        case 0b0100000000:
          std::cerr << "slt";
          break;
        case 0b0110000000:
          std::cerr << "sltu";
          break;
        default:
          assert(0);
      }
      std::cerr << " x" << rd << " x" << rs1 << " x" << rs2;
      break;
    case IA:
      std::cerr << "(I) ";
      switch(funct) {
        case 0b0000000000:
          std::cerr << "addi";
          break;
        case 0b1110000000:
          std::cerr << "andi";
          break;
        case 0b1100000000:
          std::cerr << "ori";
          break;
        case 0b1000000000:
          std::cerr << "xori";
          break;
        case 0b0100000000:
          std::cerr << "slti";
          break;
        case 0b0110000000:
          std::cerr << "sltiu";
          break;
        default:
          assert(0);
      }
      std::cerr << " x" << rd << " x" << rs1 << " " << immediate;
      break;
    case Istar:
      std::cerr << "(I*) ";
      switch (funct) {
        case 0b0010000000:
          std::cerr << "slli";
          break;
        case 0b1010000000:
          std::cerr << "srli";
          break;
        case 0b1010100000:
          std::cerr << "srai";
          break;
        default:
          assert(0);
      }
      std::cerr << " x" << rd << " x" << rs1 << " " << immediate;
      break;
    case IM:
      std::cerr << "(I) ";
      switch (funct) {
        case 0b0000000000:
          std::cerr << "lb";
          break;
        case 0b1000000000:
          std::cerr << "lbu";
          break;
        case 0b0010000000:
          std::cerr << "lh";
          break;
        case 0b1010000000:
          std::cerr << "lhu";
          break;
        case 0b0100000000:
          std::cerr << "lw";
          break;
        default:
          assert(0);
      }
      std::cerr << " x" << rd << " " << immediate << "(x" << rs1 << ")";
      break;
    case IC:
      assert(funct == 0b0000000000);
      std::cerr << "(I) jalr x" << rd << " x" << rs1 << " " << immediate;
      break;
    case S:
      std::cerr << "(S) ";
      switch (funct) {
        case 0b0000000000:
          std::cerr << "sb";
          break;
        case 0b0010000000:
          std::cerr << "sh";
          break;
        case 0b0100000000:
          std::cerr << "sw";
          break;
        default:
          assert(0);
      }
      std::cerr << " x" << rs2 << " " << immediate << "(x" << rs1 << ")";
      break;
    case B:
      std::cerr << "(B) ";
      switch (funct) {
        case 0b0000000000:
          std::cerr << "beq";
          break;
        case 0b1010000000:
          std::cerr << "bge";
          break;
        case 0b1110000000:
          std::cerr << "bgeu";
          break;
        case 0b1000000000:
          std::cerr << "blt";
          break;
        case 0b1100000000:
          std::cerr << "bltu";
          break;
        case 0b0010000000:
          std::cerr << "bne";
          break;
        default:
          assert(0);
      }
      std::cerr << " x" << rs1 << " x" << rs2 << " " << immediate;
      break;
    case Uauipc:
      std::cerr << "(U) auipc x" << rd << " " << immediate;
      break;
    case Ului:
      std::cerr << "(U) lui x" << rd << " " << immediate;
      break;
    case J:
      std::cerr << "(J) jal x" << rd << " " << immediate;
      break;
    default:
      assert(0);
  }
}

void Instruction::ExtendSign(int bit) {
  if (immediate >> bit & 1) {
    for (int i = bit + 1; i < 32; ++i) {
      immediate |= 1u << i;
    }
  }
}


InstructionParser::InstructionParser() {
#ifdef DEBUG
  debug_mode_ = true;
#else
  debug_mode_ = false;
#endif
}

// left open, right closed
uint32_t InstructionParser::Extract(const uint32_t &code, const int &l, const int &r) {
  return r == 32 ? code >> l : (code & ((1u << r) - 1u)) >> l;
}

Instruction InstructionParser::Decode(const uint32_t &address, const uint32_t &code) {
  Instruction res;
  uint32_t opcode = Extract(code, 0, 7);
  switch (opcode) {
    case 0b0110011:
      res.type = R;
      res.rd = Extract(code, 7, 12);
      res.rs1 = Extract(code, 15, 20);
      res.rs2 = Extract(code, 20, 25);
      res.funct = Extract(code, 12, 15) << 7 | Extract(code, 25, 32);
      break;
    case 0b0010011:
      res.funct = Extract(code, 12, 15);
      if (res.funct == 0b001 || res.funct == 0b101) {
        res.funct <<= 7;
        res.funct |= Extract(code, 25, 32);
        res.type = Istar;
        res.rd = Extract(code, 7, 12);
        res.rs1 = Extract(code, 15, 20);
        res.immediate = Extract(code, 20, 25);
      } else {
        res.funct <<= 7;
        res.type = IA;
        res.rd = Extract(code, 7, 12);
        res.rs1 = Extract(code, 15, 20);
        res.immediate = Extract(code, 20, 32);
        res.ExtendSign(11);
      }
      break;
    case 0b0000011:
      res.type = IM;
      res.rd = Extract(code, 7, 12);
      res.rs1 = Extract(code, 15, 20);
      res.immediate = Extract(code, 20, 32);
      res.funct = Extract(code, 12, 15) << 7;
      res.ExtendSign(11);
      break;
    case 0b1100111:
      res.type = IC;
      res.rd = Extract(code, 7, 12);
      res.rs1 = Extract(code, 15, 20);
      res.immediate = Extract(code, 20, 32);
      res.funct = Extract(code, 12, 15) << 7;
      res.ExtendSign(11);
      break;
    case 0b0100011:
      res.type = S;
      res.rs1 = Extract(code, 15, 20);
      res.rs2 = Extract(code, 20, 25);
      res.immediate = Extract(code, 25, 32) << 5 | Extract(code, 7, 12);
      res.funct = Extract(code, 12, 15) << 7;
      res.ExtendSign(11);
      break;
    case 0b1100011:
      res.type = B;
      res.rs1 = Extract(code, 15, 20);
      res.rs2 = Extract(code, 20, 25);
      res.immediate = Extract(code, 31, 32) << 12;
      res.immediate |= Extract(code, 7, 8) << 11;
      res.immediate |= Extract(code, 25, 31) << 5;
      res.immediate |= Extract(code, 8, 12) << 1;
      res.funct = Extract(code, 12, 15) << 7;
      res.ExtendSign(12);
      break;
    case 0b0010111:
      res.type = Uauipc;
      res.rd = Extract(code, 7, 12);
      res.immediate = Extract(code, 12, 32) << 12;
      break;
    case 0b0110111:
      res.type = Ului;
      res.rd = Extract(code, 7, 12);
      res.immediate = Extract(code, 12, 32) << 12;
      break;
    case 0b1101111:
      res.type = J;
      res.rd = Extract(code, 7, 12);
      res.immediate = Extract(code, 31, 32) << 20;
      res.immediate |= Extract(code, 12, 20) << 12;
      res.immediate |= Extract(code, 20, 21) << 11;
      res.immediate |= Extract(code, 21, 31) << 1;
      res.ExtendSign(20);
      break;
    default:
      assert(0);
  }
  if (debug_mode_) {
    std::cerr << address << ": ";
    res.Print();
    std::cerr << '\n';
  }
  return res;
}

}