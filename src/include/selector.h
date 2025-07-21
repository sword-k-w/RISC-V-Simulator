#ifndef SELECTOR_H
#define SELECTOR_H

#include <cstdint>

namespace sjtu {

class Selector {
private:
  uint32_t wire0_;
  uint32_t wire1_;
  bool sel_;
public:
  Selector() : wire0_(0u), wire1_(0u), sel_(false) {}
  uint32_t Result() const;
  void SetWire0(const uint32_t &);
  void SetWire1(const uint32_t &);
  void SetSel(const bool &);
};

}

#endif //SELECTOR_H
