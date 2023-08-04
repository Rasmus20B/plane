#pragma once

#include <array>
#include <cstdint>

namespace dml {
struct Task {
  std::array<char, 2000> mem{};
  std::array<float, 16> vars{};
  uint16_t pc = 0;
  uint16_t sp = 0;
  uint16_t waitctr = 0;

  void set_entry(uint16_t ep);
};
}
