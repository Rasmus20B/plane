#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>

#include "../../planes2/enm.h"
#include "coroutine.h"

namespace dml {
struct Task {

  Task() = default;

  std::array<uint8_t, 200> mem{};
  std::array<float, 16> vars{};
  std::array<plane::BulletMgr, 16> bm;
  std::array<bool, 16> live_bms;
  plane::Enm* e = nullptr;
  uint16_t cur_co = 0;
  std::array<uint16_t, 8> pc{};
  std::array<uint16_t, 8> sp{};
  std::array<uint16_t, 8> waitctr{};
  std::array<bool, 8> live_cos;
  bool base = 0;
  std::vector<uint16_t> children;

  void set_entry(uint16_t ep);
  void add_coroutine(uint16_t ep);
};
}
