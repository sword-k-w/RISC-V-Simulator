#ifndef SIMULATOR_H
#define SIMULATOR_H
#include <cstdint>
#include "Tomasulo_module/modules.h"
#include "Tomasulo_module/arithmetic_logic_unit.h"
#include "Tomasulo_module/instruction_parser.h"
#include "Tomasulo_module/load_store_buffer.h"
#include "Tomasulo_module/memory.h"
#include "Tomasulo_module/register_file.h"
#include "Tomasulo_module/reorder_buffer.h"
#include "Tomasulo_module/reservation_station.h"
#include "Tomasulo_module/predictor.h"

namespace sjtu {

class Simulator {
public:
  void Init();
  void Run();
private:
  uint64_t clock_;
  ArithmeticLogicUnit alu_[2];
  LoadStoreBuffer lsb_[2];
  Memory mem_[2];
  RegisterFile rf_[2];
  ReorderBuffer rob_[2];
  ReservationStation rs_[2];
  Predictor predictor_;
};

}

#endif //SIMULATOR_H
