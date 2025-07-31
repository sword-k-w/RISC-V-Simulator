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
  virtual bool Predict(const uint16_t &) const = 0;
  virtual void Feedback(const uint16_t &, const bool &, const bool &) = 0;
  virtual void Report() const = 0;
};

class SaturatingPredictor : public Predictor {
  friend class Memory;
  friend class ReorderBuffer;
public:
  SaturatingPredictor();
  bool Predict(const uint16_t &) const override;
  void Feedback(const uint16_t &, const bool &, const bool &) override;
  void Report() const override;
private:
  int32_t state_[4096];
  uint32_t success_cnt_ = 0;
  uint32_t total_cnt_ = 0;
};

class TwoLevelAdaptivePredictor : public Predictor  {
  friend class Memory;
  friend class ReorderBuffer;
public:
  TwoLevelAdaptivePredictor();
  bool Predict(const uint16_t &) const override;
  void Feedback(const uint16_t &, const bool &, const bool &) override;
  void Report() const override;
private:
  uint8_t history_[4096];
  int32_t state_[4096][4];
  uint32_t success_cnt_ = 0;
  uint32_t total_cnt_ = 0;
};

}

#endif //PREDICTOR_H
