#pragma once

#include <array>
#include <vector>
#include <iostream> 

#include "tasking.h"
#include "enemy.h"

namespace plane {

#define MAX_WAIT 3

  inline void wait_for(size_t ms) {
    for(int i = 0; i < ms; i+=MAX_WAIT) {
      e_mgr.paused.wait(1);
      std::this_thread::sleep_for(std::chrono::milliseconds(MAX_WAIT));
    }
  }

  inline void enemies1(void *) {
    for(size_t i = 0; i < 3; ++i) {
      e_mgr.task_queue.push(Task{
          .routine = [i](void*) {
            Enemy e(Enemy<1>{
                .id = i,
                .pos = {(config.screen_width/ 2), 0 }, 
                .mpat{ {
                  {Vector2{10, 400}, 0}, 
                  {Vector2{0, 100}, 0}, 
                  {Vector2{float(config.screen_width) + 20, 100}, 0}, 
                  {Vector2{600, 400}, 0}, 
                  {Vector2{700, 200}, 0}, 
                  {Vector2{800, 40}, 0} }, 0 },
                .prog = 0,
                .spline_t = 0.01, 
                .shoot_t = 0.7f, 
                .last_shot = 0, 
                .speed = 5.0f, 
                .size = 20, 
                .lt = {static_cast<float>(i), static_cast<float>(i)+20}, 
                .dead = false,
                .points{},
                });
            e_mgr.data->list.emplace_back(e);
            e_mgr.data->head++;
            std::cout << i << "\n";
          },
          .state = {}
          });
    }
  }
  static inline Task Stage1 {
    .routine = [](void*) {
        e_mgr.task_queue.push(Task{
            .routine = &enemies1,
            .state = {}
            }
        );
        wait_for(20);
    }
  };
}
