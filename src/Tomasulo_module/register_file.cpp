#include "Tomasulo_module/register_file.h"
#include <cstring>

namespace sjtu {

RegisterFile::RegisterFile() {
  memset(dependence_, -1, sizeof(dependence_));
}

void RegisterFile::Run() {

  if (whether_dependence_) {
    dependence_[new_reg_id] = new_dependence_;
  }
}



}