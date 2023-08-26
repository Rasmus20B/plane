#include "task.h"

namespace dml {
  void Task::set_entry(uint16_t ep) {
    this->pc[this->cur_co] = ep;
  }
  void add_coroutine(uint16_t ep) {

  }
}
