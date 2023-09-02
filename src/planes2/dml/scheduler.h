#pragma once
#include <cstdint>
#include <array>
#include <iostream>

#include "task.h"

namespace dml {
struct Scheduler {
  uint32_t n_tasks = 1;
  std::array<Task, 128> tasks;
  std::array<bool, 128> tasks_mask;
  std::mutex m;
  float total_duration;
  float cur_slice = 0;
  uint16_t c_task = 0;

  void init();
  void set_ts_duration(float ts);
  bool add_task(uint16_t ep);
  bool add_task(uint16_t ep, uint16_t base,  plane::Enm& e);
  bool add_task(uint16_t ep, float x, float y, float health, float score, float items);
  void del_task();
  bool next_task();
};
}
