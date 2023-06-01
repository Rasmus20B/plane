#include "stage1.h"

namespace plane {


  void *enem1() {
    for(int i = 0; i < 3; i++) {
      std::cout << "REKT BRUV\n";
      tmgr.paused.wait(1);
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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

