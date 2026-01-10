#include "Tomasulo_module/instruction_parser.h"

#include <inttypes.h>

namespace sjtu {

void Instruction::Print(std::ostream &os) const {
  switch (format_type) {
    case R:
      os << "(R) ";
      switch (type) {
        case Add:
          os << "add";
          break;
        case Sub:
          os << "sub";
          break;
        case And:
          os << "and";
          break;
        case Or:
          os << "or";
          break;
        case Xor:
          os << "xor";
          break;
        case Sll:
          os << "sll";
          break;
        case Srl:
          os << "srl";
          break;
        case Sra:
          os << "sra";
          break;
        case Slt:
          os << "slt";
          break;
        case Sltu:
          os << "sltu";
          break;
        default:
          assert(0);
      }
      os << " x" << rd << " x" << rs1 << " x" << rs2;
      break;
    case IA:
      os << "(I) ";
      switch (type) {
        case Addi:
          os << "addi";
          break;
        case Andi:
          os << "andi";
          break;
        case Ori:
          os << "ori";
          break;
        case Xori:
          os << "xori";
          break;
        case Slti:
          os << "slti";
          break;
        case Sltiu:
          os << "sltiu";
          break;
        default:
          assert(0);
      }
      os << " x" << rd << " x" << rs1 << " " << immediate;
      break;
    case Istar:
      os << "(I*) ";
      switch (type) {
        case Slli:
          os << "slli";
          break;
        case Srli:
          os << "srli";
          break;
        case Srai:
          os << "srai";
          break;
        default:
          assert(0);
      }
      os << " x" << rd << " x" << rs1 << " " << immediate;
      break;
    case IM:
      os << "(I) ";
      switch (type) {
        case Lb:
          os << "lb";
          break;
        case Lbu:
          os << "lbu";
          break;
        case Lh:
          os << "lh";
          break;
        case Lhu:
          os << "lhu";
          break;
        case Lw:
          os << "lw";
          break;
        default:
          assert(0);
      }
      os << " x" << rd << " " << immediate << "(x" << rs1 << ")";
      break;
    case IC:
      os << "(I) jalr x" << rd << " x" << rs1 << " " << immediate;
      break;
    case S:
      os << "(S) ";
      switch (type) {
        case Sb:
          os << "sb";
          break;
        case Sh:
          os << "sh";
          break;
        case Sw:
          os << "sw";
          break;
        default:
          assert(0);
      }
      os << " x" << rs2 << " " << immediate << "(x" << rs1 << ")";
      break;
    case B:
      os << "(B) ";
      switch (type) {
        case Beq:
          os << "beq";
          break;
        case Bge:
          os << "bge";
          break;
        case Bgeu:
          os << "bgeu";
          break;
        case Blt:
          os << "blt";
          break;
        case Bltu:
          os << "bltu";
          break;
        case Bne:
          os << "bne";
          break;
        default:
          assert(0);
      }
      os << " x" << rs1 << " x" << rs2 << " " << immediate;
      break;
    case U:
      os << "(U) ";
      if (type == Auipc) {
        os << "auipc";
      } else {
        assert(type == Lui);
        os << "lui";
      }
      os << " x" << rd << " " << immediate;
      break;
    case J:
      os << "(J) jal x" << rd << " " << immediate;
      break;
    default:
      assert(0);
  }
}

void Instruction::ExtendSign(int32_t bit) {
  if (immediate >> bit & 1) {
    for (int32_t i = bit + 1; i < 32; ++i) {
      immediate |= 1u << i;
    }
  }
}


InstructionParser::InstructionParser() {
#ifdef LOCAL
  debug_mode_ = true;
#else
  debug_mode_ = false;
#endif
}

// left open, right closed
uint32_t InstructionParser::Extract(const uint32_t &code, const int32_t &l, const int32_t &r) {
  return r == 32 ? code >> l : (code & ((1u << r) - 1u)) >> l;
}

Instruction InstructionParser::Decode(const uint32_t &address, const uint32_t &code) {
  Instruction res;
  uint32_t opcode = Extract(code, 0, 7);
  uint32_t funct;
  res.rd = Extract(code, 7, 12);
  res.rs1 = Extract(code, 15, 20);
  res.rs2 = Extract(code, 20, 25);
  switch (opcode) {
    case 0b0110011:
      res.format_type = R;
      funct = Extract(code, 12, 15) << 7 | Extract(code, 25, 32);
      switch (funct) {
        case 0b0000000000:
          res.type = Add;
          break;
        case 0b0000100000:
          res.type = Sub;;
          break;
        case 0b1110000000:
          res.type = And;
          break;
        case 0b1100000000:
          res.type = Or;
          break;
        case 0b1000000000:
          res.type = Xor;
          break;
        case 0b0010000000:
          res.type = Sll;
          break;
        case 0b1010000000:
          res.type = Srl;
          break;
        case 0b1010100000:
          res.type = Sra;
          break;
        case 0b0100000000:
          res.type = Slt;
          break;
        case 0b0110000000:
          res.type = Sltu;
          break;
        default:
          assert(0);
      }
      break;
    case 0b0010011:
      funct = Extract(code, 12, 15);
      if (funct == 0b001 || funct == 0b101) {
        res.format_type = Istar;
        res.immediate = Extract(code, 20, 25);
        funct <<= 7;
        funct |= Extract(code, 25, 32);
        switch (funct) {
          case 0b0010000000:
            res.type = Slli;
            break;
          case 0b1010000000:
            res.type = Srli;
            break;
          case 0b1010100000:
            res.type = Srai;
            break;
          default:
            assert(0);
        }
      } else {
        res.format_type = IA;
        res.immediate = Extract(code, 20, 32);
        res.ExtendSign(11);
        switch (funct) {
          case 0b000:
            res.type = Addi;
            break;
          case 0b111:
            res.type = Andi;
            break;
          case 0b110:
            res.type = Ori;
            break;
          case 0b100:
            res.type = Xori;
            break;
          case 0b010:
            res.type = Slti;
            break;
          case 0b011:
            res.type = Sltiu;
            break;
          default:
            assert(0);
        }
      }
      break;
    case 0b0000011:
      res.format_type = IM;
      res.immediate = Extract(code, 20, 32);
      funct = Extract(code, 12, 15);
      switch (funct) {
        case 0b000:
          res.type = Lb;
          break;
        case 0b100:
          res.type = Lbu;
          break;
        case 0b001:
          res.type = Lh;
          break;
        case 0b101:
          res.type= Lhu;
          break;
        case 0b010:
          res.type = Lw;
          break;
        default:
          assert(0);
      }
      res.ExtendSign(11);
      break;
    case 0b1100111:
      res.format_type = IC;
      res.ExtendSign(11);
      assert(Extract(code, 12, 15) == 0b000);
      res.type = Jalr;
      break;
    case 0b0100011:
      res.format_type = S;
      res.immediate = Extract(code, 25, 32) << 5 | Extract(code, 7, 12);
      res.ExtendSign(11);
      funct = Extract(code, 12, 15);
      switch (funct) {
        case 0b000:
          res.type = Sb;
          break;
        case 0b001:
          res.type = Sh;
          break;
        case 0b010:
          res.type = Sw;
          break;
        default:
          assert(0);
      }
      break;
    case 0b1100011:
      res.format_type = B;
      res.immediate = Extract(code, 31, 32) << 12;
      res.immediate |= Extract(code, 7, 8) << 11;
      res.immediate |= Extract(code, 25, 31) << 5;
      res.immediate |= Extract(code, 8, 12) << 1;
      res.ExtendSign(12);
      funct = Extract(code, 12, 15);
      switch (funct) {
        case 0b000:
          res.type = Beq;
          break;
        case 0b101:
          res.type = Bge;
          break;
        case 0b111:
          res.type = Bgeu;
          break;
        case 0b100:
          res.type = Blt;
          break;
        case 0b110:
          res.type = Bltu;
          break;
        case 0b001:
          res.type = Bne;
          break;
        default:
          assert(0);
      }
      break;
    case 0b0010111:
      res.format_type = U;
      res.type = Auipc;
      res.immediate = Extract(code, 12, 32) << 12;
      break;
    case 0b0110111:
      res.format_type = U;
      res.type = Lui;
      res.immediate = Extract(code, 12, 32) << 12;
      break;
    case 0b1101111:
      res.format_type = J;
      res.type = Jal;
      res.immediate = Extract(code, 31, 32) << 20;
      res.immediate |= Extract(code, 12, 20) << 12;
      res.immediate |= Extract(code, 20, 21) << 11;
      res.immediate |= Extract(code, 21, 31) << 1;
      res.ExtendSign(20);
      break;
    default:
      assert(0);
  }
  res.address_hash_val = address & 4095;
  return res;
}

}
