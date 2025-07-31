#ifndef LOAD_STORE_BUFFER_H
#define LOAD_STORE_BUFFER_H

#include <cstdint>

#include "instruction_parser.h"
#include "Tomasulo_module/modules.h"

namespace sjtu {

struct LoadStoreBufferEntry {
  Instruction instruction;
  bool ready;
  uint32_t dest;
  uint32_t address;
  uint32_t value;
};

class LoadStoreBuffer {
  friend class ArithmeticLogicUnit;
  friend class ReorderBuffer;
  friend class Memory;
private:
  bool predict_failed_ = false;

  uint32_t las_rob_tail_;
  bool whether_new_instruction_ = false;
  Instruction new_instruction_;

  int32_t alu_broadcast_dest_ = -1;
  uint32_t alu_broadcast_address_;

  int32_t rob_broadcast_dest_ = -1;
  uint32_t rob_broadcast_address_;
  uint32_t rob_broadcast_value_;

  int32_t waiting_cycle_ = -1;
  uint32_t head_ = 0;
  uint32_t tail_ = 0;
  LoadStoreBufferEntry entry[32];

  void PassInstruction();

public:
  Memory *mem_;
  void Run();
  void Copy(const LoadStoreBuffer &);
};

}

#endif //LOAD_STORE_BUFFER_H
