#include "simulator.h"

namespace sjtu {

void Simulator::Init() {
  mem_[0].Init();
  alu_[0].lsb_ = &lsb_[1];
  alu_[0].rob_ = &rob_[1];
  alu_[0].rs_ = &rs_[1];
  lsb_[0].mem_ = &mem_[1];
  mem_[0].predictor_ = &predictor_;
  mem_[0].rf_ = &rf_[1];
  mem_[0].rob_ = &rob_[1];
  mem_[0].rs_ = &rs_[1];
  rf_[0].rs_ = &rs_[1];
  rob_[0].lsb_ = &lsb_[1];
  rob_[0].mem_ = &mem_[1];
  rob_[0].predictor_ = &predictor_;
  rob_[0].rf_ = &rf_[1];
  rob_[0].rs_ = &rs_[1];
  rs_[0].alu_ = &alu_[1];
}

void Simulator::Run() {
  ++clock_;
  std::cerr << "[" << clock_ << "]\n";
  if (clock_ == 30) {
    exit(0);
  }

  alu_[0].Run();
  lsb_[0].Run();
  mem_[0].RunPC();
  mem_[0].RunMemory();
  rob_[0].Run();
  rs_[0].Run();
  rf_[0].Run();

  alu_[0].Copy(alu_[1]);
  lsb_[0].Copy(lsb_[1]);
  mem_[0].Copy(mem_[1]);
  rob_[0].Copy(rob_[1]);
  rs_[0].Copy(rs_[1]);
  rf_[0].Copy(rf_[1]);
  std::cerr << '\n';
}

}