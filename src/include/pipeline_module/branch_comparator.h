#ifndef BRANCH_COMPARATOR_H
#define BRANCH_COMPARATOR_H
#include <cstdint>
#include <utility>

namespace sjtu {

class BranchComparator {
public:
  BranchComparator();
  std::pair<bool, bool> Result() const;
  void SetWire0(const uint32_t &);
  void SetWire1(const uint32_t &);
  void SetBrUn(const uint32_t &);
private:
  uint32_t wire0_;
  uint32_t wire1_;
  bool is_unsigned_;
};

}

#endif //BRANCH_COMPARATOR_H
