#include <cstdint>
#include <array>
#include <iostream>

#include "task.h"

#pragma once


namespace dml {
struct Scheduler {
  uint32_t n_tasks = 1;
  std::array<Task, 128> tasks;
  std::array<bool, 128> tasks_mask;
  float total_duration = 0.002;
  float cur_slice = 0;
  uint16_t c_task = 0;

  void init();
  void set_ts_duration(float ts);
  bool add_task(uint16_t ep);
  void del_task();
  bool next_task();
};
}
