#include "scheduler.h"

namespace dml {
  void Scheduler::init() {
    std::fill(tasks_mask.begin(), tasks_mask.end(), false);
    tasks_mask[0] = true;
    Task t;
    t.set_entry(0);
    tasks[0] = t;
  }

  void Scheduler::set_ts_duration(float ts) {
    this->total_duration = ts;
    this->cur_slice = this->total_duration / this->n_tasks;
  }

  bool Scheduler::add_task(uint16_t ep) {

    uint16_t i = this->c_task + 1;
    while(tasks_mask[i]) {
      if(i >= this->tasks_mask.size() - 1) {
        i = 0;
      }
      if(i == this->c_task) {
        return false;
      }
      i++;
    }
    this->tasks_mask[i] = true;
    Task t;
    std::fill(t.mem.begin(), t.mem.end(), 0);
    t.set_entry(ep);
    this->tasks[i] = t;
    this->n_tasks++;
    this->cur_slice = this->total_duration / (this->n_tasks) ;
    return true;
  }
  
  void Scheduler::del_task() {
    this->n_tasks--;
    this->cur_slice = this->total_duration / this->n_tasks;
    tasks_mask[c_task] = false;
  }

  bool Scheduler::next_task() {
    uint16_t i = c_task + 1;
    while(!this->tasks_mask[i]) {
      if(i >= this->tasks_mask.size() - 1) {
        i = 0;
        continue;
      }
      if(i == c_task) {
        if(!tasks_mask[c_task]) {
          // no active tasks, lets shut down
          return false;
          // else we stick with the same one
        } else break;
      }
      i++;
    }
    this->c_task = i;
    return true;
  }
}
