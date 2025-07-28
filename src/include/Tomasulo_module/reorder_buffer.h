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
};

class ReorderBuffer {
  friend class Memory;
  friend class ArithmeticLogicUnit;
private:
  bool whether_new_instruction_ = false;
  Instruction new_instruction_;

  int broadcast_dest_ = -1;
  uint32_t broadcast_val_;

  uint32_t head_ = 0;
  uint32_t tail_ = 0;
  RoBEntry entry_[32];
public:
  Memory *mem_;
  ReservationStation *rs_;
  RegisterFile *rf_;
  LoadStoreBuffer *lsb_;
  Predictor *predictor_;
  void Run();
  void Copy(const ReorderBuffer &);
};

}

#endif //REORDER_BUFFER_H
