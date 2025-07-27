#include "Tomasulo_module/instruction_parser.h"

#include <inttypes.h>

namespace sjtu {

void Instruction::Print() const {
  switch (format_type) {
    case R:
      std::cerr << "(R) ";
      switch (type) {
        case Add:
          std::cerr << "add";
          break;
        case Sub:
          std::cerr << "sub";
          break;
        case And:
          std::cerr << "and";
          break;
        case Or:
          std::cerr << "or";
          break;
        case Xor:
          std::cerr << "xor";
          break;
        case Sll:
          std::cerr << "sll";
          break;
        case Srl:
          std::cerr << "srl";
          break;
        case Sra:
          std::cerr << "sra";
          break;
        case Slt:
          std::cerr << "slt";
          break;
        case Sltu:
          std::cerr << "sltu";
          break;
        default:
          assert(0);
      }
      std::cerr << " x" << rd << " x" << rs1 << " x" << rs2;
      break;
    case IA:
      std::cerr << "(I) ";
      switch (type) {
        case Addi:
          std::cerr << "addi";
          break;
        case Andi:
          std::cerr << "andi";
          break;
        case Ori:
          std::cerr << "ori";
          break;
        case Xori:
          std::cerr << "xori";
          break;
        case Slti:
          std::cerr << "slti";
          break;
        case Sltiu:
          std::cerr << "sltiu";
          break;
        default:
          assert(0);
      }
      std::cerr << " x" << rd << " x" << rs1 << " " << immediate;
      break;
    case Istar:
      std::cerr << "(I*) ";
      switch (type) {
        case Slli:
          std::cerr << "slli";
          break;
        case Srli:
          std::cerr << "srli";
          break;
        case Srai:
          std::cerr << "srai";
          break;
        default:
          assert(0);
      }
      std::cerr << " x" << rd << " x" << rs1 << " " << immediate;
      break;
    case IM:
      std::cerr << "(I) ";
      switch (type) {
        case Lb:
          std::cerr << "lb";
          break;
        case Lbu:
          std::cerr << "lbu";
          break;
        case Lh:
          std::cerr << "lh";
          break;
        case Lhu:
          std::cerr << "lhu";
          break;
        case Lw:
          std::cerr << "lw";
          break;
        default:
          assert(0);
      }
      std::cerr << " x" << rd << " " << immediate << "(x" << rs1 << ")";
      break;
    case IC:
      std::cerr << "(I) jalr x" << rd << " x" << rs1 << " " << immediate;
      break;
    case S:
      std::cerr << "(S) ";
      switch (type) {
        case Sb:
          std::cerr << "sb";
          break;
        case Sh:
          std::cerr << "sh";
          break;
        case Sw:
          std::cerr << "sw";
          break;
        default:
          assert(0);
      }
      std::cerr << " x" << rs2 << " " << immediate << "(x" << rs1 << ")";
      break;
    case B:
      std::cerr << "(B) ";
      switch (type) {
        case Beq:
          std::cerr << "beq";
          break;
        case Bge:
          std::cerr << "bge";
          break;
        case Bgeu:
          std::cerr << "bgeu";
          break;
        case Blt:
          std::cerr << "blt";
          break;
        case Bltu:
          std::cerr << "bltu";
          break;
        case Bne:
          std::cerr << "bne";
          break;
        default:
          assert(0);
      }
      std::cerr << " x" << rs1 << " x" << rs2 << " " << immediate;
      break;
    case U:

      std::cerr << "(U) ";
      if (type == Auipc) {
        std::cerr << "auipc";
      } else {
        assert(type == Lui);
        std::cerr << "Lui";
      }
      std::cerr << " x" << rd << " " << immediate;
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
  uint32_t funct;
  switch (opcode) {
    case 0b0110011:
      res.format_type = R;
      res.rd = Extract(code, 7, 12);
      res.rs1 = Extract(code, 15, 20);
      res.rs2 = Extract(code, 20, 25);
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
        res.rd = Extract(code, 7, 12);
        res.rs1 = Extract(code, 15, 20);
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
        res.rd = Extract(code, 7, 12);
        res.rs1 = Extract(code, 15, 20);
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
      res.rd = Extract(code, 7, 12);
      res.rs1 = Extract(code, 15, 20);
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
      res.rd = Extract(code, 7, 12);
      res.rs1 = Extract(code, 15, 20);
      res.immediate = Extract(code, 20, 32);
      res.ExtendSign(11);
      assert(Extract(code, 12, 15) == 0b000);
      res.type = Jalr;
      break;
    case 0b0100011:
      res.format_type = S;
      res.rs1 = Extract(code, 15, 20);
      res.rs2 = Extract(code, 20, 25);
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
      res.rs1 = Extract(code, 15, 20);
      res.rs2 = Extract(code, 20, 25);
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
      res.rd = Extract(code, 7, 12);
      res.immediate = Extract(code, 12, 32) << 12;
      break;
    case 0b0110111:
      res.format_type = U;
      res.rd = Extract(code, 7, 12);
      res.immediate = Extract(code, 12, 32) << 12;
      break;
    case 0b1101111:
      res.format_type = J;
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
