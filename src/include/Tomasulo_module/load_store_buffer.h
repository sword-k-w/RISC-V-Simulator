#ifndef LOAD_STORE_BUFFER_H
#define LOAD_STORE_BUFFER_H

#include <cstdint>

#include "Tomasulo_module/modules.h"

namespace sjtu {

class LoadStoreBuffer {
  friend class ArithmeticLogicUnit;
  friend class ReorderBuffer;
private:
  bool predict_failed_ = false;

  uint32_t head_ = 0;
  uint32_t tail_ = 0;

  ReorderBuffer *old_rob_;
public:
  LoadStoreBuffer();
  void Run();
  void Copy(const LoadStoreBuffer &);
};

}

#endif //LOAD_STORE_BUFFER_H
