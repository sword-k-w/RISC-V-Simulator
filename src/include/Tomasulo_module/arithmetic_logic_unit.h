#ifndef ARITHMETIC_LOGIC_UNIT_H
#define ARITHMETIC_LOGIC_UNIT_H

#include <cstdint>
#include <cassert>
#include "Tomasulo_module/modules.h"
#include "Tomasulo_module/reservation_station.h"

namespace sjtu {

class ArithmeticLogicUnit {
  friend class ReservationStation;
  friend class ReorderBuffer;
private:
  bool predict_failed_ = false;
  bool is_zero_ = false;

  int32_t dest_ = -1;
  uint32_t wireA_;
  uint32_t wireB_;
  uint32_t wireS_;
  InstructionType sel_;

public:
  ReorderBuffer *rob_;
  ReservationStation *rs_;
  LoadStoreBuffer *lsb_;
  void Run();
  void Copy(const ArithmeticLogicUnit &);
};

}

#endif //ARITHMETIC_LOGIC_UNIT_H
