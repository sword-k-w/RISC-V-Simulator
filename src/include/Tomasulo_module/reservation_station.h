#ifndef RESERVATION_STATION_H
#define RESERVATION_STATION_H

#include "Tomasulo_module/instruction_parser.h"
#include "Tomasulo_module/modules.h"
#include "Tomasulo_module/arithmetic_logic_unit.h"

namespace sjtu {

struct ReservationStationEntry {
  InstructionType op_type_;
  bool busy_ = false;
  uint32_t val1_;
  uint32_t val2_;
  int32_t depend1_{-1};
  int32_t depend2_{-1};
  uint32_t dest_;
};

class ReservationStation {
private:
  Instruction new_instruction_;

  ReservationStationEntry entry_[32];

  ArithmeticLogicUnit* alu_;
public:
  void Run();
  void Copy(const ReservationStation &);
};

}

#endif //RESERVATION_STATION_H
