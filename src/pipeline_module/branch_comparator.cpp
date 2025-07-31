#include "pipeline_module/branch_comparator.h"

namespace sjtu {

BranchComparator::BranchComparator() : wire0_(0u), wire1_(0u), is_unsigned_(true) {}

std::pair<bool, bool> BranchComparator::Result() const {
  if (is_unsigned_) {
    return std::make_pair(wire0_ == wire1_, wire0_ < wire1_);
  }
  return std::make_pair(wire0_ == wire1_, static_cast<int32_t>(wire0_) < static_cast<int32_t>(wire1_));
}

void BranchComparator::SetWire0(const uint32_t &input) {
  wire0_ = input;
}

void BranchComparator::SetWire1(const uint32_t &input) {
  wire1_ = input;
}

void BranchComparator::SetBrUn(const uint32_t &is_unsigned) {
  is_unsigned_ = is_unsigned;
}

}