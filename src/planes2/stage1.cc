#include "stage1.h" 

namespace plane {
  void load_stage1enemies(TextureManager& tm,EnemyPool& ep) {

    for(int i = 0; i < 1; ++i) {
      auto tmp =  Spline( {
        {{config.screen_width / 3, -200}, 0},
        {{(config.screen_width / 2) , 0}, 0},
        {{(config.screen_width / 4) * i + (config.screen_width / 10), 100}, 0},
        {{(config.screen_width / 4) * i, 200}, 0},
        }, 0).calc_points(0.01f, 5.0f, false);
      addEnemy(ep, std::move(Enemy{
        .points = tmp,
        .col = GREEN,
        .size = 20,
        .health = 1,
        .speed = 5.0f,
        .spawntime = static_cast<float>(i),
        .sprite = tm.textures[8],
        .looped = false,
        }));

      tmp =  Spline( {
        {{config.screen_width / 3, 200}, 0},
        {{(config.screen_width / 2) , 0}, 0},
        {{(config.screen_width / 4) * -i + (config.screen_width / 10) * 9, 100}, 0},
        {{(config.screen_width / 4) * -i, 200}, 0},
        }, 0).calc_points(0.01f, 5.0f, false);
      addEnemy(ep, std::move(Enemy{
        .points = tmp,
        .col = GREEN,
        .size = 20,
        .health = 1,
        .speed = 5.0f,
        .spawntime = static_cast<float>(i),
        .sprite = tm.textures[8],
        .looped = false,
        }));

      tmp =  Spline( {
        {{config.screen_width / 2, 0}, 0},
        {{config.screen_width / 2, 100}, 0},
        {{config.screen_width / 2, 100}, 0},
        {{config.screen_width / 2, 200}, 0},
        }, 0).calc_points(0.01f, 5.0f, false);
      addEnemy(ep, std::move(Enemy{
        .points = tmp,
        .col = GREEN,
        .size = 20,
        .health = 1,
        .speed = 5.0f,
        .spawntime = static_cast<float>(i),
        .sprite = tm.textures[8],
        .looped = false,
        }));
    }
  }
  void load_stage1projectiles(ProjectilePool& pp);
}
