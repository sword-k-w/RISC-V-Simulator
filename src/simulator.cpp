#include "simulator.h"

#include <random>

namespace sjtu {

void Simulator::Init(int32_t argc, char **argv) {
  if (argc == 2 && strcmp(argv[1], "local") == 0) {
    predictor_ = new LocalPredictor();
  } else if (argc == 2 && strcmp(argv[1], "global") == 0) {
    predictor_ = new GlobalPredictor();
  } else if (argc == 2 && strcmp(argv[1], "two_level_adaptive") == 0) {
    predictor_ = new TwoLevelAdaptivePredictor();
  } else if (argc == 2 && strcmp(argv[1], "tournament") == 0) {
    predictor_ = new TournamentPredictor();
  } else {
    predictor_ = new TwoLevelAdaptivePredictor();
  }
  mem_[0].Init();
  alu_[0].lsb_ = &lsb_[1];
  alu_[0].rob_ = &rob_[1];
  alu_[0].rs_ = &rs_[1];
  lsb_[0].mem_ = &mem_[1];
  lsb_[0].rob_ = &rob_[1];
  mem_[0].predictor_ = predictor_;
  mem_[0].rf_ = &rf_[1];
  mem_[0].rob_ = &rob_[1];
  mem_[0].rs_ = &rs_[1];
  mem_[0].lsb_ = &lsb_[1];
  rf_[0].rs_ = &rs_[1];
  rob_[0].alu_ = &alu_[1];
  rob_[0].lsb_ = &lsb_[1];
  rob_[0].mem_ = &mem_[1];
  rob_[0].predictor_ = predictor_;
  rob_[0].rf_ = &rf_[1];
  rob_[0].rs_ = &rs_[1];
  rob_[0].other_ = &rob_[1];
  rs_[0].alu_ = &alu_[1];
}

std::mt19937 rnd(time(nullptr));

void Simulator::Run() {
  ++clock_;
  // std::cerr << "clock = " << clock_ << '\n';

  int32_t p[7] = {0, 1, 2, 3, 4, 5, 6};
  std::shuffle(p, p + 7, rnd);
  for (int32_t i = 0; i < 7; ++i) {
    if (p[i] == 0) {
      alu_[0].Run();
    } else if (p[i] == 1) {
      lsb_[0].Run();
    } else if (p[i] == 2) {
      mem_[0].RunPC();
    } else if (p[i] == 3) {
      mem_[0].RunMemory();
    } else if (p[i] == 4) {
      rs_[0].Run();
    } else if (p[i] == 5) {
      rf_[0].Run();
    } else if (rob_[0].Run()) {
      std::cerr << "Total Cycle Count = " << clock_ << '\n';
      predictor_->Report();
      std::cout << rf_[0].Result() << '\n';
      delete predictor_;
      exit(0);
    }
  }

  // std::cerr << '\n';
  alu_[0].Copy(alu_[1]);
  lsb_[0].Copy(lsb_[1]);
  mem_[0].Copy(mem_[1]);
  rob_[0].Copy(rob_[1]);
  rs_[0].Copy(rs_[1]);
  rf_[0].Copy(rf_[1]);
}

}
