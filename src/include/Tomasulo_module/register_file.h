#ifndef REGISTER_FILE_H
#define REGISTER_FILE_H

#include "Tomasulo_module/modules.h"
#include <cstdint>

namespace sjtu {

class RegisterFile {
  friend class ReservationStation;
  friend class Memory;
private:
  bool whether_dependence_ = false;
  uint32_t new_reg_id;
  int32_t new_dependence_;

  uint32_t reg_[32];
  int32_t dependence_[32]; // -1 means no dependence
public:
  RegisterFile();
  void Run();
  void Copy(const RegisterFile &);
};

}

#endif //REGISTER_FILE_H
