#include "Tomasulo_module/predictor.h"

namespace sjtu {

LocalPredictor::LocalPredictor() {
  for (int32_t i = 0; i < 4096; ++i) {
    state_[i] = 2;
  }
}

auto LocalPredictor::Predict(const uint16_t &address_hash_val) -> uint8_t const {
  return state_[address_hash_val] < 3;
}

void LocalPredictor::Feedback(const uint16_t &address_hash_val, const bool &result, const uint8_t &predict) {
  ++total_cnt_;
  if (result == (predict & 1)) {
    ++success_cnt_;
  }
  if (result) {
    state_[address_hash_val] = std::max(state_[address_hash_val] - 1, 1);
  } else {
    state_[address_hash_val] = std::min(state_[address_hash_val] + 1, 4);
  }
}

auto GlobalPredictor::Predict(const uint16_t &address_hash_val) -> uint8_t const {
  return state_ < 3;
}

void LocalPredictor::Report() const {
  std::cerr << "Predict Information:\n";
  std::cerr << "Curren predictor is local predictor.\n";
  std::cerr << "Predict " << total_cnt_ << " times and success " << success_cnt_ << '\n';
  std::cerr << "Predict Accuracy = " << std::fixed << std::setprecision(2) << 100.0 * success_cnt_ / total_cnt_ << "%\n";
}

void GlobalPredictor::Feedback(const uint16_t &address_hash_val, const bool &result, const uint8_t &predict) {
  ++total_cnt_;
  if (result == (predict & 1)) {
    ++success_cnt_;
  }
  if (result) {
    state_ = std::max(state_ - 1, 1);
  } else {
    state_ = std::min(state_ + 1, 4);
  }
}

void GlobalPredictor::Report() const {
  std::cerr << "Predict Information:\n";
  std::cerr << "Curren predictor is global predictor.\n";
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

auto TwoLevelAdaptivePredictor::Predict(const uint16_t &address_hash_val) -> uint8_t  const {
  return state_[address_hash_val][history_[address_hash_val]] < 3;
}

void TwoLevelAdaptivePredictor::Feedback(const uint16_t &address_hash_val, const bool &result, const uint8_t &predict) {
  ++total_cnt_;
  if (result == (predict & 1)) {
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

TournamentPredictor::TournamentPredictor() {
  for (int32_t i = 0; i < 4096; ++i) {
    local_state_[i] = 2;
  }
}

auto TournamentPredictor::Predict(const uint16_t &address_hash_val) -> uint8_t const {
  uint8_t res = 0;
  res |= (local_state_[address_hash_val] < 3) << 2;
  res |= (global_state_ < 3) << 1;
  res |= choose_state_ < 3 ? local_state_[address_hash_val] < 3 : global_state_ < 3;
  return res;
}

void TournamentPredictor::Feedback(const uint16_t &address_hash_val, const bool &result, const uint8_t &predict) {
  ++total_cnt_;
  if (result == (predict & 1)) {
    ++success_cnt_;
  }
  if (result) {
    local_state_[address_hash_val] = std::max(local_state_[address_hash_val] - 1, 1);
    global_state_ = std::max(global_state_, 1);
  } else {
    local_state_[address_hash_val] = std::min(local_state_[address_hash_val] + 1, 4);
    global_state_ = std::min(global_state_, 4);
  }
  bool local_success = result == (predict >> 2 & 1);
  bool global_success = result == (predict >> 1 & 1);
  if (local_success && !global_success) {
    choose_state_ = std::max(choose_state_ - 1, 1);
  } else if (!local_success && global_success) {
    choose_state_ = std::min(choose_state_ + 1, 4);
  }
}

void TournamentPredictor::Report() const {
  std::cerr << "Predict Information:\n";
  std::cerr << "Curren predictor is tournament predictor.\n";
  std::cerr << "Predict " << total_cnt_ << " times and success " << success_cnt_ << '\n';
  std::cerr << "Predict Accuracy = " << std::fixed << std::setprecision(2) << 100.0 * success_cnt_ / total_cnt_ << "%\n";
}

}