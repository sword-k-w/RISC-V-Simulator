#ifndef REORDER_BUFFER_H
#define REORDER_BUFFER_H
#include "Tomasulo_module/modules.h"

namespace sjtu {

class ReorderBuffer {
private:

public:
  void Run();
  void Copy(const ReorderBuffer &);
};

}

#endif //REORDER_BUFFER_H
