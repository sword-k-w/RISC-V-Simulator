#include "Tomasulo_module/predictor.h"

namespace sjtu {

SaturatingPredictor::SaturatingPredictor() {
  for (int32_t i = 0; i < 4096; ++i) {
    state_[i] = 2;
  }
}

bool SaturatingPredictor::Predict(const uint16_t &address_hash_val) const {
  return state_[address_hash_val] < 3;
}

void SaturatingPredictor::Feedback(const uint16_t &address_hash_val, const bool &result, const bool &predict) {
  ++total_cnt_;
  if (result == predict) {
    ++success_cnt_;
  }
  if (result) {
    state_[address_hash_val] = std::max(state_[address_hash_val] - 1, 1);
  } else {
    state_[address_hash_val] = std::min(state_[address_hash_val] + 1, 4);
  }
}

void SaturatingPredictor::Report() const {
  std::cerr << "Predict Information:\n";
  std::cerr << "Curren predictor is saturating predictor.\n";
  std::cerr << "Predict " << total_cnt_ << " times and success " << success_cnt_ << '\n';
  std::cerr << "Predict Accuracy = " << std::fixed << std::setprecision(2) << 100.0 * success_cnt_ / total_cnt_ << "%\n";
}

TwoLevelAdaptivePredictor::TwoLevelAdaptivePredictor() {
  for (int32_t i = 0; i < 4096; ++i) {
    history_[i] = 0;
    for (int32_t j = 0; j < 4; ++j) {
      state_[i][j] = 2;
    }
  }
}

bool TwoLevelAdaptivePredictor::Predict(const uint16_t &address_hash_val) const {
  return state_[address_hash_val][history_[address_hash_val]] < 3;
}

void TwoLevelAdaptivePredictor::Feedback(const uint16_t &address_hash_val, const bool &result, const bool &predict) {
  ++total_cnt_;
  if (result == predict) {
    ++success_cnt_;
  }
  if (result) {
    state_[address_hash_val][history_[address_hash_val]] = std::max(state_[address_hash_val][history_[address_hash_val]] - 1, 1);
  } else {
    state_[address_hash_val][history_[address_hash_val]] = std::min(state_[address_hash_val][history_[address_hash_val]] + 1, 4);
  }
  history_[address_hash_val] <<= 1;
  history_[address_hash_val] |= result;
  history_[address_hash_val] &= 3;
}

void TwoLevelAdaptivePredictor::Report() const {
  std::cerr << "Predict Information:\n";
  std::cerr << "Curren predictor is two-level adaptive predictor.\n";
  std::cerr << "Predict " << total_cnt_ << " times and success " << success_cnt_ << '\n';
  std::cerr << "Predict Accuracy = " << std::fixed << std::setprecision(2) << 100.0 * success_cnt_ / total_cnt_ << "%\n";
}

}