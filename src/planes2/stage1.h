#pragma once

#include "enemy.h"
#include "projectile.h"
#include "../common/config.h"

#include "../common/spline.h"

namespace plane {
    void load_stage1enemies(EnemyPool& ep);
    void load_stage1projectiles(ProjectilePool& pp);
}
