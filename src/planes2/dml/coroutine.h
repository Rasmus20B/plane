#pragma once

#include <sched.h>

#include <cstdint>
#include <array>

namespace dml {


struct Coroutine {
  std::array<uint8_t, 256> stack;
  uint16_t c_task = 0;
  uint16_t pc{};
  uint16_t sp{};

  void init();
  void set_ts_duration(float ts);
};

struct CoManager {
  std::array<Coroutine, 8> coroutines;
  std::array<bool, 8> live_mask;

  void addCoroutine(uint16_t ep);
  void delCoroutine();
};

}
