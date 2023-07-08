#pragma once

#include <vector>

#include <raylib.h>


#ifdef MULTI_T
#include <thread>
#include <mutex>
#endif

#include <chrono> 
#include <iostream>
#include <cmath>

#include "../common/vector_calc.h"
#include "../common/config.h"

#include "collision.h"
#include "enm.h"
#include "input.h"
#include "move.h"
#include "player.h"
#include "projectile.h"
#include "texture.h"
#include "timeline.h"

#include "stage1/stage1.h"

namespace plane {
  void main_loop();
}
