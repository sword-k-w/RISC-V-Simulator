#ifndef ARITHMETIC_LOGIC_UNIT_H
#define ARITHMETIC_LOGIC_UNIT_H

#include <cstdint>
#include <cassert>
#include "Tomasulo_module/modules.h"
#include "Tomasulo_module/reservation_station.h"

namespace sjtu {

enum ArithmeticType {
  Add, Sub, And, Or, Xor, Sll, Srl, Sra, Slt, Sltu
};

class ArithmeticLogicUnit {
  friend class ReservationStation;
private:
  uint32_t wireA_;
  uint32_t wireB_;
  ArithmeticType sel_;
public:
  ArithmeticLogicUnit();
  void Run();
  void Copy(const ArithmeticLogicUnit &);
};

}

#endif //ARITHMETIC_LOGIC_UNIT_H
