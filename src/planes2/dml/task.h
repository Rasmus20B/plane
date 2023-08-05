#pragma once

#include <array>
#include <cstdint>

#include "../../planes2/enm.h"

namespace dml {
struct Task {
  std::array<char, 2000> mem{};
  std::array<float, 16> vars{};
  plane::Enm e;
  std::array<plane::BulletMgr, 16> bm;
  uint16_t pc = 0;
  uint16_t sp = 0;
  uint16_t waitctr = 0;

  void set_entry(uint16_t ep);
};
}
