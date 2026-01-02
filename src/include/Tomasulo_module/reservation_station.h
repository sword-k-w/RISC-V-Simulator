#ifndef RESERVATION_STATION_H
#define RESERVATION_STATION_H

#include <cstring>

#include "reorder_buffer.h"
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
  int32_t immediate_S;
  bool is_zero = false;
};

class ReservationStation {
  friend class ReorderBuffer;
  friend class ReservationStation;
  friend class Memory;
  friend class ArithmeticLogicUnit;
  friend class RegisterFile;
public:
  ArithmeticLogicUnit* alu_;
  void CheckDependence(uint32_t &, int32_t &, const uint32_t &);
  void Run();
  void Copy(const ReservationStation &);
private:
  bool predict_failed_ = false;

  uint32_t las_rob_tail_;
  bool whether_new_instruction_ = false;
  Instruction new_instruction_;

  int32_t alu_broadcast_dest_ = -1;
  uint32_t alu_broadcast_val_;

  int32_t lsb_broadcast_dest_ = -1;
  uint32_t lsb_broadcast_val_;

  uint32_t old_reg_[32];
  int32_t old_dependence_[32];

  RoBEntry old_rob_entry_[32];

  ReservationStationEntry entry_[32];
};

}

#endif //RESERVATION_STATION_H
