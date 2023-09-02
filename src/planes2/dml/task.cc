#include "task.h"

namespace dml {
  void Task::set_entry(uint16_t ep) {
    this->pc[this->cur_co] = ep;
  }

  void Task::add_coroutine(uint16_t ep) {
    for(int i = 0; i < this->live_cos.size(); ++i) {
      if(live_cos[i] == false) {
        this->pc[i] = ep;
        this->live_cos[i] = true;
        this->waitctr[i] = 0;
      }
    }
  }
}
