#ifndef ARITHMETIC_LOGIC_UNIT_H
#define ARITHMETIC_LOGIC_UNIT_H

#include <cstdint>
#include <cassert>

namespace sjtu {

enum ArithmeticType {
  Add, Sub, And, Or, Xor, Sll, Srl, Sra, Slt, Sltu
};

class ArithmeticLogicUnit {
private:
  uint32_t wireA_;
  uint32_t wireB_;
  ArithmeticType sel_;
public:
  ArithmeticLogicUnit();
  uint32_t Result() const;
  void SetWireA(const uint32_t &);
  void SetWireB(const uint32_t &);
  void SetSel(const ArithmeticType &);
};

}

#endif //ARITHMETIC_LOGIC_UNIT_H
