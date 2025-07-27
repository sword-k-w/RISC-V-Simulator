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

namespace sjtu {

class Simulator {
private:
  uint32_t clock_;
public:
  void Run();
};

}

#endif //SIMULATOR_H
