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
  friend class LoadStoreBuffer;
private:
  bool thaw_ = false;

  bool predict_failed_ = false;
  uint32_t new_pc_;

  InstructionType new_type_;
  uint32_t new_address_;
  uint32_t new_value_;

  uint32_t las_rob_head_;
  uint32_t las_rob_tail_;

  bool whether_commit_ = false;
  InstructionType commit_type_;
  uint32_t commit_address_;
  uint32_t commit_value_;
  uint32_t commit_dest_;

  bool jalr_frozen_ = false;
  InstructionParser instruction_parser_;
  uint32_t pc_ = 0;
  std::unordered_map<uint32_t, uint32_t> memory_;

public:
  ReorderBuffer *rob_;
  ReservationStation *rs_;
  RegisterFile *rf_;
  Predictor *predictor_;
  Memory();
  void Init();
  void RunPC();
  void RunMemory();
  void Copy(const Memory &);
  uint32_t GetInstruction();
};

}

#endif //MEMORY_H
