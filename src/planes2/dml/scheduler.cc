#include "scheduler.h"

namespace dml {
  void Scheduler::init() {
    std::fill(tasks_mask.begin(), tasks_mask.end(), false);
    tasks_mask[0] = true;
    Task t;
    t.set_entry(0);
    c_task = 0;
    tasks[0] = t;
  }

  void Scheduler::set_ts_duration(float ts) {
    this->total_duration = ts;
    this->cur_slice = this->total_duration / this->n_tasks;
  }

  bool Scheduler::add_task(uint16_t ep) {

    uint16_t i = 0;
    while(i < this->tasks_mask.size()) {
      if(this->tasks_mask[i] == false) {
        Task t;
        t.set_entry(ep);
        this->tasks[i] = t;
        this->tasks_mask[i] = true;
        this->n_tasks++;
        this->cur_slice = this->total_duration / (this->n_tasks) ;
        return true;
      }
      ++i;
    }
    return false;
  }
  
  void Scheduler::del_task() {
    this->n_tasks--;
    this->cur_slice = this->total_duration / this->n_tasks;
    tasks_mask[c_task] = false;
  }

  bool Scheduler::next_task() {
    uint16_t i = (c_task + 1) % this->tasks_mask.size();
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
