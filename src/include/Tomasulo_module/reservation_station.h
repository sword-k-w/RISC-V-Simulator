#ifndef RESERVATION_STATION_H
#define RESERVATION_STATION_H

#include "Tomasulo_module/instruction_parser.h"
#include "Tomasulo_module/modules.h"
#include "Tomasulo_module/arithmetic_logic_unit.h"

namespace sjtu {

struct ReservationStationEntry {
  InstructionType type;
  bool busy = false;
  uint32_t val1;
  uint32_t val2;
  int32_t depend1{-1};
  int32_t depend2{-1};
  uint32_t dest;
};

class ReservationStation {
  friend class ReorderBuffer;
  friend class ReservationStation;
  friend class Memory;
  friend class ArithmeticLogicUnit;
private:
  bool predict_failed_ = false;

  uint32_t las_rob_head_;
  uint32_t las_rob_tail_;
  bool whether_new_instruction_ = false;
  Instruction new_instruction_;

  int broadcast_dest_;
  uint32_t broadcast_val_;

  ReservationStationEntry entry_[32];

public:
  RegisterFile* old_rf_;
  ArithmeticLogicUnit* alu_;
  void Run();
  void Copy(const ReservationStation &);
};

}

#endif //RESERVATION_STATION_H
