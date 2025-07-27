#ifndef LOAD_STORE_BUFFER_H
#define LOAD_STORE_BUFFER_H

#include "Tomasulo_module/modules.h"

namespace sjtu {

class LoadStoreBuffer {
  friend class ArithmeticLogicUnit;
private:

  ReorderBuffer *old_rob_;
public:
  LoadStoreBuffer();
  void Run();
  void Copy(const LoadStoreBuffer &);
};

}

#endif //LOAD_STORE_BUFFER_H
