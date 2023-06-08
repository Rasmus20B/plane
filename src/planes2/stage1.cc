#include "stage1.h" 

namespace plane {
  void stage1(EnemyPool& ep) {
    for(int i = 0; i < 3; ++i) {
      addEnemy(ep, Enemy{
          .size = 20,
          .health = 1,
          })
    }
  }
}
