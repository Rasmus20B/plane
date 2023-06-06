#pragma once

#include <array>
#include <vector>
#include <iostream> 

#include "tasking.h"
#include "enemy.h"
#include "config.h"

namespace plane {

#define MAX_WAIT 3

  inline void wait_for(size_t ms) {
    for(int i = 0; i < ms; i+=MAX_WAIT) {
      e_mgr.paused.wait(1);
      std::this_thread::sleep_for(std::chrono::milliseconds(MAX_WAIT));
    }
  }

  inline void enemies1(void *) {
    for(size_t i = 0; i < 1; ++i) {
      e_mgr.task_queue.push(std::move(Task{
          .routine = [i](void*) {
            e_mgr.data->list.emplace_back(Enemy<1>{
                .id = static_cast<size_t>(i),
                .pos = {(config.screen_width/ 2), 0 }, 
                .pts{ Spline(
                    std::vector<SplinePt>{
                      {Vector2{10, 400}, 0}, 
                      {Vector2{0, 100}, 0}, 
                      {Vector2{float(config.screen_width) + 20, 100}, 0}, 
                      {Vector2{600, 400}, 0}, 
                      {Vector2{700, 200}, 0}, 
                      {Vector2{800, 40}, 0} }, 5).calc_points(0.01f, 5.0f)
                    },
                .prog = 0,
                .spline_t = 0.01, 
                .shoot_t = 0.7f, 
                .last_shot = 0, 
                .speed = 5.0f, 
                .size = 20, 
                .lt = {static_cast<float>(i), static_cast<float>(i)+20}, 
                .dead = false,
                .draw = std::shared_ptr<std::atomic<bool>>(new std::atomic<bool>(false))
                });

            e_mgr.data->head++;
            size_t c = 0;
            while(!e_mgr.data->list[i].dead) {
              e_mgr.data->list[i].draw->wait(true);
              if(c >= e_mgr.data->list[i].pts.size()) c = 0;
              e_mgr.data->list[i].pos = e_mgr.data->list[i].pts[c];
              c++;
              e_mgr.data->list[i].draw->store(true);
            }


          },
          .state = {}
          }));
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
