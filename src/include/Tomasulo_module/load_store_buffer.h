#ifndef LOAD_STORE_BUFFER_H
#define LOAD_STORE_BUFFER_H

#include "Tomasulo_module/modules.h"

class LoadStoreBuffer {
private:

public:
  LoadStoreBuffer();
  void Run();
  void Copy(const LoadStoreBuffer &);
};

#endif //LOAD_STORE_BUFFER_H
