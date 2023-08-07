#pragma once

#include <array>
#include <cstdint>

#include "../../planes2/enm.h"
#include "enm_data.h"

namespace dml {
struct Task {
  std::array<char, 2000> mem{};
  std::array<float, 16> vars{};
  std::array<plane::BulletMgr, 16> bm;
  std::array<bool, 16> live_bms;
  plane::Enm e;
  uint16_t pc = 0;
  uint16_t sp = 0;
  uint16_t waitctr = 0;

  void set_entry(uint16_t ep);
};
}
