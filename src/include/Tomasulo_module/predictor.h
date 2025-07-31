#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <algorithm>
#include <iomanip>
#include <Tomasulo_module/modules.h>
#include <Tomasulo_module/memory.h>
#include <Tomasulo_module/reorder_buffer.h>

namespace sjtu {

class Predictor {
  friend class Memory;
  friend class ReorderBuffer;
public:
  virtual ~Predictor() = default;
  virtual auto Predict(const uint16_t &) -> uint8_t const = 0;
  virtual void Feedback(const uint16_t &, const bool &, const uint8_t &) = 0;
  virtual void Report() const = 0;
};

class LocalPredictor : public Predictor {
public:
  LocalPredictor();
  auto Predict(const uint16_t &) -> uint8_t const override;
  void Feedback(const uint16_t &, const bool &, const uint8_t &) override;
  void Report() const override;
private:
  int32_t state_[4096];
  uint32_t success_cnt_ = 0;
  uint32_t total_cnt_ = 0;
};

class GlobalPredictor : public Predictor {
public:
  auto Predict(const uint16_t &) -> uint8_t const override;
  void Feedback(const uint16_t &, const bool &, const uint8_t &) override;
  void Report() const override;
private:
  int32_t state_ = 2;
  uint32_t success_cnt_ = 0;
  uint32_t total_cnt_ = 0;
};

class TwoLevelAdaptivePredictor : public Predictor  {
public:
  TwoLevelAdaptivePredictor();
  auto Predict(const uint16_t &) -> uint8_t const override;
  void Feedback(const uint16_t &, const bool &, const uint8_t &) override;
  void Report() const override;
private:
  uint8_t history_[4096];
  int32_t state_[4096][4];
  uint32_t success_cnt_ = 0;
  uint32_t total_cnt_ = 0;
};

class TournamentPredictor : public Predictor {
public:
  TournamentPredictor();
  auto Predict(const uint16_t &) -> uint8_t const override;
  void Feedback(const uint16_t &, const bool &, const uint8_t &) override;
  void Report() const override;
private:
  int32_t choose_state_ = 2;
  int32_t local_state_[4096];
  int32_t global_state_ = 2;
  uint32_t success_cnt_ = 0;
  uint32_t total_cnt_ = 0;
};

}

#endif //PREDICTOR_H
