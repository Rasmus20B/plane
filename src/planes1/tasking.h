#pragma once

#include <cstdint>

#include <thread>
#include <mutex>
#include <functional> 

#include <memory> 

#include "../common/queue.h"

#include <thread>

namespace plane {

enum TaskState {
  TASK_STATE_RUNNING,
  TASK_STATE_PENDING,
  TASK_STATE_FINISHED
};

enum TaskManagerState {
  TASK_MGR_STATE_RUNNING,
  TASK_MGR_STATE_STOPPED
};

struct Task {
  std::function<void(void*)> routine;
  TaskState state;
};

template<typename T>
struct TaskManager {
  TaskManager() {
    n_threads = 20;
    for(int i = 0; i < n_threads; i++) {
      threads.push_back(std::thread(&TaskManager::taskmgr_thread, this));
    }
  }

  ~TaskManager() {
    status = TASK_MGR_STATE_STOPPED;
    for(auto &t: threads) {
      t.detach();
    }
  }

  void taskmgr_thread() {
    auto state = this->status;
    while(state == TASK_MGR_STATE_RUNNING) {
      state = this->status;
      auto otask = task_queue.pop_and_get();
      Task task;
      if(otask.has_value()) task = otask.value();
      if(task.routine) {
        task.routine({});
      }
      else this->work.wait(true);
    }
  }
  std::atomic<uint32_t> n_tasks{};
  std::mutex queue_lock;
  Queue<Task, 256> task_queue;
  std::vector<std::thread> threads;
  TaskManagerState status;
  std::atomic<bool> paused = false;
  std::atomic<bool> work = false;
  T* data;
  uint16_t n_threads;
};
}
