#ifndef INSTRUCTION_PARSER_H
#define INSTRUCTION_PARSER_H
#include <cstdint>
#include <iostream>
#include <cassert>

namespace sjtu {

enum InstructionType {
  R, IA, Istar, IM, IC, S, B, Uauipc, Ului, J
};

struct Instruction {
  InstructionType type;
  uint32_t rd;
  uint32_t rs1;
  uint32_t rs2;
  uint32_t immediate;
  uint32_t funct;
  void Print() const;
};

class InstructionParser {
private:
  bool debug_mode_;

  uint32_t Extract(const uint32_t &, const int &, const int &);
public:
  InstructionParser();
  Instruction Decode(const uint32_t &, const uint32_t &);
};

}

#endif //INSTRUCTION_PARSER_H
