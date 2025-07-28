#ifndef MEMORY_H
#define MEMORY_H
#include "Tomasulo_module/modules.h"
#include "Tomasulo_module/predictor.h"
#include <unordered_map>
#include <cstdint>

#include "instruction_parser.h"

namespace sjtu {

class Memory {
  friend class ReorderBuffer;
private:
  bool predict_failed_ = false;
  uint32_t new_pc_;

  uint32_t las_rob_head_;
  uint32_t las_rob_tail_;

  InstructionParser instruction_parser_;
  uint32_t pc_ = 0;
  Instruction cur_instruction_;
  std::unordered_map<uint32_t, uint32_t> memory_;

  ReorderBuffer *rob_;
  ReservationStation *rs_;
  RegisterFile *rf_;
  Predictor *predictor_;
public:
  Memory();
  void RunPC();
  void RunMemory();
  void Copy(const Memory &);
  uint32_t GetInstruction();
};

}

#endif //MEMORY_H
