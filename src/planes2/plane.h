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
#include <cmath>

#include "../common/vector_calc.h"
#include "../common/config.h"

#include "enemy.h"
#include "player.h"
#include "projectile.h"
#include "stage1.h"

namespace plane {
  void main_loop();
}
