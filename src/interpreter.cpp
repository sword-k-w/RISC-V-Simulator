#include "interpreter.h"

namespace sjtu {

Interpreter::Interpreter() : pc_(0u) {}

void Interpreter::Run() {
  while (true) {
    // PC
    IMEM_.SetPC(pc_);
    selectorA_.SetWire1(pc_);
    selectorPC_.SetWire0(pc_ + 4);
    selectorWB1_.SetWire1(pc_ + 4);

    // IMEM
    uint32_t code = IMEM_.Result();
    Instruction instruction = parser_.Decode(pc_, code);
    ImmGen_.SetCode(code);
    ImmGen_.SetSel(instruction.type);
    if (instruction.type == Ului) {
      RegFile_.SetReadIndex1(0u);
    } else {
      RegFile_.SetReadIndex1(InstructionParser::Extract(code, 15, 20));
    }
    RegFile_.SetReadIndex2(InstructionParser::Extract(code, 20, 25));

    // ImmGen
    selectorB_.SetWire1(ImmGen_.Result());

    // RegFile
    std::pair<uint32_t, uint32_t> tmp_data = RegFile_.Result();
    RegFile_.SetWriteIndex(InstructionParser::Extract(code, 7, 12));
    RegFile_.SetWriteEnable(instruction.type != S && instruction.type != B);
    selectorA_.SetWire0(tmp_data.first);
    BranchComp_.SetWire0(tmp_data.first);
    selectorB_.SetWire0(tmp_data.second);
    BranchComp_.SetWire1(tmp_data.second);
    DMEM_.SetInput(tmp_data.second);

    if (code == 0x0ff00513) {
      std::cout << RegFile_.GetReturnValue() << '\n';
      return;
    }

    // BranchComp
    BranchComp_.SetBrUn(instruction.funct >> 1 & 1);
    std::pair<bool, bool> branch_res = BranchComp_.Result();

    // selectorA
    bool Asel = false;
    if (instruction.type == B || instruction.type == J || instruction.type == Uauipc) {
      Asel = true;
    }
    selectorA_.SetSel(Asel);
    ALU_.SetWireA(selectorA_.Result());

    // selectorB
    bool Bsel = true;
    if (instruction.type == R) {
      Bsel = false;
    }
    selectorB_.SetSel(Bsel);
    ALU_.SetWireB(selectorB_.Result());

    // ALU
    if (instruction.type == Ului) {
      ALU_.SetSel(Add);
    } else {
      switch (instruction.funct) {
        case 0b0000000000:
          ALU_.SetSel(Add);
        break;
        case 0b0000100000:
          ALU_.SetSel(Sub);
        break;
        case 0b1110000000:
          ALU_.SetSel(And);
        break;
        case 0b1100000000:
          ALU_.SetSel(Or);
        break;
        case 0b1000000000:
          ALU_.SetSel(Xor);
        break;
        case 0b0010000000:
          ALU_.SetSel(Sll);
        break;
        case 0b1010000000:
          ALU_.SetSel(Srl);
        break;
        case 0b1010100000:
          ALU_.SetSel(Sra);
        break;
        case 0b0100000000:
          ALU_.SetSel(Slt);
        break;
        case 0b0110000000:
          ALU_.SetSel(Sltu);
        break;
        default:
          ALU_.SetSel(Add);
      }
    }
    uint32_t alu_res = ALU_.Result();
    DMEM_.SetAddress(alu_res);
    selectorWB0_.SetWire1(alu_res);
    selectorPC_.SetWire1(alu_res);

    // DMEM
    DMEM_.SetRW(instruction.type != S);
    DMEM_.SetUn(instruction.funct >> 9 & 1);
    uint32_t op = instruction.funct >> 7 & 3;
    if (op == 0) {
      DMEM_.SetSize(Byte);
    } else if (op == 1) {
      DMEM_.SetSize(Half);
    } else {
      DMEM_.SetSize(Word);
    }
    selectorWB0_.SetWire0(DMEM_.Result());

    // selectorWB
    int WBsel = 1;
    if (instruction.type == IM) {
      WBsel = 0;
    } else if (instruction.type == IC || instruction.type == J) {
      WBsel = 2;
    }
    selectorWB0_.SetSel(WBsel & 1);
    selectorWB1_.SetSel(WBsel >> 1);
    selectorWB1_.SetWire0(selectorWB0_.Result());
    RegFile_.SetInput(selectorWB1_.Result());

    // selectorPC
    bool PCsel = false;
    if (instruction.type == J || instruction.type == IC) {
      PCsel = true;
    } else if (instruction.type == B) {
      switch (instruction.funct) {
        case 0b0000000000:
          if (branch_res.first) {
            PCsel = true;
          }
        break;
        case 0b0010000000:
          if (!branch_res.first) {
            PCsel = true;
          }
        break;
        case 0b1010000000:
          if (!branch_res.second) {
            PCsel = true;
          }
        break;
        case 0b1110000000:
          if (!branch_res.second) {
            PCsel = true;
          }
        break;
        case 0b1000000000:
          if (branch_res.second) {
            PCsel = true;
          }
        break;
        case 0b1100000000:
          if (branch_res.second) {
            PCsel = true;
          }
        break;
        default:
      }
    }
    selectorPC_.SetSel(PCsel);
    pc_ = selectorPC_.Result();
  }
}

}