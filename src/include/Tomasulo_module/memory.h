#ifndef MEMORY_H
#define MEMORY_H
#include "Tomasulo_module/modules.h"
#include <unordered_map>
#include <cstdint>

namespace sjtu {

class Memory {
private:
  std::unordered_map<uint32_t, uint32_t> memory_;

public:
  void Run();
  void Copy(const Memory &);
};

}

#endif //MEMORY_H
