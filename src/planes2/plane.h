#pragma once

#include <vector>

#include <raylib.h>

#define MULTI_T

#ifdef MULTI_T
#include <thread>
#include <mutex>
#endif

#include <chrono> 
#include <iostream>

#include "../common/vector_calc.h"
#include "../common/config.h"

#include "projectile.h"
#include "player.h"
#include "enemy.h"

namespace plane {
  void main_loop();
}
