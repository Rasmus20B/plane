#include "task.h"

namespace dml {
  void Task::set_entry(uint16_t ep) {
    this->pc = ep;
  }

  void Task::add_coroutine(uint16_t ep) {
    this->pc = ep;
    this->waitctr = 0;
  }
}
