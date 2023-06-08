#pragma once

#include <array>
#include <vector>
#include <iostream> 

#include "tasking.h"
#include "enemy.h"
#include "../common/config.h"

namespace plane {

#define MAX_WAIT 3

  inline void wait_for(size_t ms) {
    for(int i = 0; i < ms; i+=MAX_WAIT) {
      e_mgr.paused.wait(1);
      std::this_thread::yield();
      std::this_thread::sleep_for(std::chrono::milliseconds(MAX_WAIT));
    }
  }

  inline void enemies1(void *) {
    for(size_t i = 0; i < 2; ++i) {
      e_mgr.task_queue.push((Task{
          .routine = [i](void*) {

            std::unique_lock<std::mutex> lock(e_mgr.data->m);
            size_t id = e_mgr.data->list.size();
            e_mgr.data->list.push_back(Enemy{
                .id = static_cast<size_t>(id),
                .pos = {(config.screen_width/ 2), 0 }, 
                .pts{ Spline(
                    std::vector<SplinePt>{
                      {{10, 0}, 0}, 
                      {{100, 100}, 0}, 
                      {{300 + (i * 50.0f), 200}, 0}, 
                      // {{600, 400}, 0}, 
                      // {{700, 200}, 0}, 
                      /*{{800, 40}, 0} */}, 5).calc_points(0.01f, 10.0f)
                    },
                .prog = 0,
                .spline_t = 0.01, 
                .shoot_t = 0.7f, 
                .last_shot = 0, 
                .speed = 7.0f, 
                .size = 20, 
                .lt = {static_cast<float>(i), static_cast<float>(id)+20}, 
                .dead = false,
                .draw = std::make_unique<std::atomic_flag>(false)
                });
            lock.unlock();
            size_t c = 0;
            while(!e_mgr.data->list[id].dead) {
              e_mgr.data->list[id].draw->wait(true);
              if(c >= e_mgr.data->list[id].pts.size()) c = 0;
              e_mgr.data->list[id].pos = e_mgr.data->list[id].pts[c];
              c++;
              e_mgr.data->list[id].draw->test_and_set();
              wait_for(3);
            }
          },
          .state = {}
      }));

      e_mgr.task_queue.push((Task{
          .routine = [i](void*) {

            std::unique_lock<std::mutex> lock(e_mgr.data->m);
            size_t id = e_mgr.data->list.size();
            e_mgr.data->list.push_back(Enemy{
                .id = static_cast<size_t>(id),
                .pos = {(config.screen_width/ 2), 0 }, 
                .pts{ Spline(
                    std::vector<SplinePt>{
                      {{config.screen_width - 10, 0}, 0}, 
                      {{config.screen_width - 100, 100}, 0}, 
                      {{config.screen_width - 300 - (i * 50.0f), 200}, 0}
                      // {{float(config.screen_width) + 20, 100}, 0}, 
                      // {{600, 400}, 0}, 
                      // {{700, 200}, 0}, 
                      /*{ {800, 40}, 0} */ }, 5).calc_points(0.01f, 10.0f)
                    },
                .prog = 0,
                .spline_t = 0.01, 
                .shoot_t = 0.7f, 
                .last_shot = 0, 
                .speed = 7.0f, 
                .size = 20, 
                .lt = {static_cast<float>(i), static_cast<float>(i)+20}, 
                .dead = false,
                .draw = std::make_unique<std::atomic_flag>(false)
                });
            lock.unlock();
            size_t c = 0;
            while(!e_mgr.data->list[id].dead) {
              e_mgr.data->list[id].draw->wait(true);
              if(c >= e_mgr.data->list[id].pts.size() - e_mgr.data->list[id].speed) c = 0;
              e_mgr.data->list[id].pos = e_mgr.data->list[id].pts[c];
              c++;
              e_mgr.data->list[id].draw->test_and_set();
              wait_for(3);
            }
          },
          .state = {}
      }));
      wait_for(20);
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
