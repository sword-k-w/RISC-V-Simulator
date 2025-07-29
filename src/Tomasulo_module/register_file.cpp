#include "Tomasulo_module/register_file.h"
#include <cstring>
#include <Tomasulo_module/reservation_station.h>

namespace sjtu {

RegisterFile::RegisterFile() {
  memset(dependence_, -1, sizeof(dependence_));
  memset(reg_, 0, sizeof(reg_));
}

void RegisterFile::Run() {
  if (predict_failed_) {
    for (int i = 0; i < 32; ++i) {
      dependence_[i] = -1;
    }
  }
  if (whether_commit_) {
    if (dependence_[commit_reg_id_] == commit_rob_id_) {
      dependence_[commit_reg_id_] = -1;
      if (commit_reg_id_) {
        reg_[commit_reg_id_] = commit_value_;
      }
    }
  }
  if (whether_dependence_) {
    dependence_[new_reg_id_] = new_dependence_;
  }
  memcpy(rs_->old_dependence_, dependence_, sizeof(dependence_));
  memcpy(rs_->old_reg_, reg_, sizeof(reg_));
  // std::cerr << "{RegisterFile}\n";
  // for (int i = 0; i < 32; ++i) {
  //   std::cerr << "x" << i << " : " << reg_[i] << " " << dependence_[i] << '\n';
  // }
}

void RegisterFile::Copy(const RegisterFile &other) {
  predict_failed_ = other.predict_failed_;
  whether_dependence_ = other.whether_dependence_;
  new_reg_id_ = other.new_reg_id_;
  new_dependence_ = other.new_dependence_;
  whether_commit_ = other.whether_commit_;
  commit_rob_id_ = other.commit_rob_id_;
  commit_reg_id_ = other.commit_reg_id_;
  commit_value_ = other.commit_value_;
}

uint32_t RegisterFile::Result() const {
  return reg_[10] & 255;
}


}
