#pragma once

#include <raylib.h>

#include <functional>
#include <condition_variable> 

#include "spline.h"
#include "tasking.h"

namespace plane {

struct Lifetime {
  constexpr bool contains(float t) {
    return (start <= t && end >= t);
  }
  float start;
  float end;
};

struct Enemy {
  size_t id;
  SplinePt pos;
  std::vector<SplinePt> pts;
  float prog = 0;
  float spline_t = 0.01;
  float shoot_t;
  float last_shot;
  float speed = 0.01;
  float size;
  Lifetime lt;
  bool dead = true;
  bool marked = false;

  // routine that uses a time to determine what to do, which point to go to, etc
  std::unique_ptr<std::atomic_flag> draw;
  std::function<void(float time)> draw_func;
};

struct EnemyList {
  std::vector<Enemy> list;
  std::mutex m;
  size_t head = 0;
};

inline TaskManager<EnemyList> e_mgr{};
}
