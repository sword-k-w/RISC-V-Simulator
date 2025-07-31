#ifndef INSTRUCTION_PARSER_H
#define INSTRUCTION_PARSER_H
#include <cstdint>
#include <iostream>
#include <cassert>

namespace sjtu {

enum FormatType {
  R, IA, Istar, IM, IC, S, B, U, J
};

enum InstructionType {
  Add, Sub, And, Or, Xor, Sll, Srl, Sra, Slt, Sltu,
  Addi, Andi, Ori, Xori, Slli, Srli, Srai, Slti, Sltiu,
  Lb, Lbu, Lh, Lhu, Lw, Sb, Sh, Sw,
  Beq, Bge, Bgeu, Blt, Bltu, Bne,
  Jal, Jalr, Auipc, Lui
};

struct Instruction {
  FormatType format_type;
  InstructionType type;
  uint32_t rd;
  uint32_t rs1;
  uint32_t rs2;
  int32_t immediate;
  bool predict;
  void Print(std::ostream &) const;
  void ExtendSign(int32_t bit);
};

class InstructionParser {
public:
  InstructionParser();
  static uint32_t Extract(const uint32_t &, const int32_t &, const int32_t &);
  Instruction Decode(const uint32_t &, const uint32_t &);
private:
  bool debug_mode_;
};

}

#endif //INSTRUCTION_PARSER_H
