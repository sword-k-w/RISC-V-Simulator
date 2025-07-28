#include "Tomasulo_module/predictor.h"

namespace sjtu {

bool Predictor::Predict() const {
  return state_ < 3;
}

void Predictor::Feedback(const bool &result) {
  if (result) {
    --state_;
    state_ = std::max(state_, 1);
  } else {
    ++state_;
    state_ = std::min(state_, 4);
  }
}

}