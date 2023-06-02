#include "stage1.h"

namespace plane {

#define MAX_WAIT 5

  inline void wait_for(size_t ms) {
    for(int i = 0; i < ms; i+=MAX_WAIT) {
      tmgr.paused.wait(1);
      std::this_thread::sleep_for(std::chrono::milliseconds(MAX_WAIT));
    }
  }

  void *enem1() {
    for(int i = 0; i < 3; i++) {
      std::cout << "REKT BRUV\n";
      wait_for(1000);
    }
  }

  void *stage1() {
    std::cout << "we're in t stage one heres\n";
    tmgr.task_queue.push(Task{
        .func = enem1,
        .state{},
        });

    return 0;
  }
}
//TODO : just use a routine for each enemy, regular function

