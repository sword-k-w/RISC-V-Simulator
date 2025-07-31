#ifndef REORDER_BUFFER_H
#define REORDER_BUFFER_H
#include "Tomasulo_module/modules.h"
#include <cstdint>

#include "Tomasulo_module/predictor.h"
#include "Tomasulo_module/instruction_parser.h"

namespace sjtu {

struct RoBEntry {
  Instruction instruction;
  bool ready = false;
  uint32_t value;
  uint32_t address;
};

class ReorderBuffer {
  friend class Memory;
  friend class ArithmeticLogicUnit;
public:
  ArithmeticLogicUnit *alu_;
  Memory *mem_;
  ReservationStation *rs_;
  RegisterFile *rf_;
  LoadStoreBuffer *lsb_;
  SaturatingPredictor *predictor_;
  ReorderBuffer *other_;

  auto Run() -> bool;
  void Copy(const ReorderBuffer &);
private:
  bool predict_failed = false;

  bool whether_new_instruction_ = false;
  Instruction new_instruction_;

  int32_t alu_broadcast_dest_ = -1;
  uint32_t alu_broadcast_val_;
  uint32_t alu_broadcast_address_;

  int32_t lsb_broadcast_dest_ = -1;
  uint32_t lsb_broadcast_val_;

  uint32_t head_ = 0;
  uint32_t tail_ = 0;
  RoBEntry entry_[32];
};

}

#endif //REORDER_BUFFER_H
