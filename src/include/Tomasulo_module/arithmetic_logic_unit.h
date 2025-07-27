#ifndef ARITHMETIC_LOGIC_UNIT_H
#define ARITHMETIC_LOGIC_UNIT_H

#include <cstdint>
#include <cassert>
#include "Tomasulo_module/modules.h"
#include "Tomasulo_module/reservation_station.h"

namespace sjtu {

class ArithmeticLogicUnit {
  friend class ReservationStation;
private:
  int dest_ = -1;
  uint32_t wireA_;
  uint32_t wireB_;
  InstructionType sel_;

  ReorderBuffer *rob_;
  ReservationStation *rs_;
  LoadStoreBuffer *lsb_;
public:
  ArithmeticLogicUnit();
  void Run();
  void Copy(const ArithmeticLogicUnit &);
};

}

#endif //ARITHMETIC_LOGIC_UNIT_H
