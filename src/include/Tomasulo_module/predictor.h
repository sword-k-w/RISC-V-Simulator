#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <algorithm>
#include <iomanip>
#include <Tomasulo_module/modules.h>
#include <Tomasulo_module/memory.h>
#include <Tomasulo_module/reorder_buffer.h>

namespace sjtu {

class SaturatingPredictor {
  friend class Memory;
  friend class ReorderBuffer;
public:
  SaturatingPredictor();
  bool Predict(const uint16_t &) const;
  void Feedback(const uint16_t &, const bool &, const bool &);
  void Report() const;
private:
  int32_t state_[4096];
  uint32_t success_cnt_ = 0;
  uint32_t total_cnt_ = 0;
};

}

#endif //PREDICTOR_H
