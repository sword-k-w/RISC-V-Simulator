#ifndef REGISTER_FILE_H
#define REGISTER_FILE_H

#include "Tomasulo_module/modules.h"
#include <cstdint>

namespace sjtu {

class RegisterFile {
private:
  uint32_t reg_[32];
  int32_t dependence_[32]; // -1 means no dependence
public:
  RegisterFile();
};

}

#endif //REGISTER_FILE_H
