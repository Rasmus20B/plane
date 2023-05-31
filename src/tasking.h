#pragma once

#include <cstdint>

#include <thread>
#include <mutex>

#include "queue.h"

#include <thread>

#include <iostream>
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
  std::function<void*(void)> func;
  TaskState state;
};

struct TaskManager {
  TaskManager() {
    n_threads = std::thread::hardware_concurrency();
    for(int i = 0; i < n_threads; i++) {
      threads.push_back(std::thread(&TaskManager::taskmgr_thread, this));
    }
  }

  ~TaskManager() {
    status = TASK_MGR_STATE_STOPPED;
    for(auto &t: threads) {
      t.join();
    }
  }

  void taskmgr_thread() {
    auto state = this->status;
    while(state == TASK_MGR_STATE_RUNNING) {
      state = this->status;
      auto otask = task_queue.pop_and_get();
      Task task;
      if(otask.has_value()) task = otask.value();
      if(task.func) {
        task.func();
      }
      else continue;
    }
  }
  std::atomic<uint32_t> n_tasks{};
  std::mutex queue_lock;
  Queue<Task, 256> task_queue;
  std::vector<std::thread> threads;
  TaskManagerState status;
  uint16_t n_threads;
};

inline TaskManager tmgr{};
}
