#ifndef REGISTER_FILE_H
#define REGISTER_FILE_H

#include "Tomasulo_module/modules.h"
#include <cstdint>
#include <ostream>

namespace sjtu {

class RegisterFile {
  friend class Memory;
  friend class ReorderBuffer;
public:
  ReservationStation *rs_;
  RegisterFile();
  void Run();
  void Copy(const RegisterFile &);
  auto Result() const -> uint32_t;
  void Print(std::ostream &) const;
private:
  bool predict_failed_ = false;

  bool whether_dependence_ = false;
  uint32_t new_reg_id_;
  uint32_t new_dependence_;

  bool whether_commit_ = false;
  uint32_t commit_rob_id_;
  uint32_t commit_reg_id_;
  uint32_t commit_value_;

  uint32_t reg_[32];
  int32_t dependence_[32]; // -1 means no dependence
};

}

#endif //REGISTER_FILE_H
