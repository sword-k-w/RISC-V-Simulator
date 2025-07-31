#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <algorithm>
#include <Tomasulo_module/modules.h>
#include <Tomasulo_module/memory.h>
#include <Tomasulo_module/reorder_buffer.h>

namespace sjtu {

class Predictor {
  friend class Memory;
  friend class ReorderBuffer;
private:
  int32_t state_ = 2;
public:
  bool Predict() const;
  void Feedback(const bool &);
};

}

#endif //PREDICTOR_H
