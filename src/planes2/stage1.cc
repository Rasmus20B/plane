#include "stage1.h" 

namespace plane {
  void load_stage1enemies(TextureManager& tm,EnemyPool& ep) {

      std::unordered_map<int, ProjectilePool> shots;
      Projectile p1 = {
                .position = Vec2{0, 0},
                .old_position = {0, 0},
                .velocity = Vec2{3, 7.0f},
                .angle = 135,
                .sprite = tm.textures[5],
                .mt = MoveType::MOVE_NORM,
                .live = true,
              };

      ProjectilePool pp1;
      projectilePoolInit(pp1, 4);
      addProjectile(pp1, p1);

      shots[120] = pp1;
      shots[310] = pp1;
      shots[220] = pp1;

      auto tmp =  Spline( {
        {{config.screen_width / 3, -200}, 0},
        {{(config.screen_width / 2) , 0}, 0},
        {{(config.screen_width / 4) * 1 + (config.screen_width / 10), 100}, 0},
        {{(config.screen_width / 4) * 1, 200}, 0},
        }, 0).calc_points(0.01f, 5.0f, false);
      addEnemy(ep, std::move(Enemy{
        .points = tmp,
        .shots = shots,
        .size = 20,
        .health = 1,
        .speed = 5.0f,
        .spawntime = static_cast<float>(1),
        .sprite = tm.textures[8],
        .looped = false,
        }));

      tmp =  Spline( {
        {{config.screen_width / 3, 200}, 0},
        {{(config.screen_width / 2) , 0}, 0},
        {{(config.screen_width / 4) * -2 + (config.screen_width / 10) * 9, 100}, 0},
        {{(config.screen_width / 4) * -2, 200}, 0},
        }, 0).calc_points(0.01f, 5.0f, false);

      addEnemy(ep, std::move(Enemy{
        .points = tmp,
        .shots = shots,
        .size = 20,
        .health = 1,
        .speed = 5.0f,
        .spawntime = static_cast<float>(2),
        .sprite = tm.textures[8],
        .looped = false,
        .state = EnemyState::ENEMY_MOVING,
        }));

      tmp =  Spline( {
        {{config.screen_width / 2, 0}, 0},
        {{config.screen_width / 2, 100}, 0},
        {{config.screen_width / 2, 100}, 0},
        {{config.screen_width / 2, 200}, 0},
        }, 0).calc_points(0.01f, 5.0f, false);
      addEnemy(ep, std::move(Enemy{
        .points = tmp,
        .shots = shots,
        .size = 20,
        .health = 1,
        .speed = 5.0f,
        .spawntime = static_cast<float>(3),
        .sprite = tm.textures[8],
        .looped = false,
        }));


      tmp = Spline( {
          {{config.screen_width / 2, 0}, 0},
          {{config.screen_width / 4, 300}, 0},
          {{config.screen_width / 5, 350}, 0},
          {{config.screen_width / 4, 400}, 0}
          }, 0).calc_points(0.01f, 5.0f, false);


      std::unordered_map<int, ProjectilePool> big;

      ProjectilePool pp2;
      projectilePoolInit(pp2, 8);
      for(int i = 0; i < 8; ++i) {
        Projectile p2 = {
          .velocity = {4, 4},
          .angle = (360.0f / 8) * i,
          .angle_inc = 0,
          .sprite = tm.textures[4],
          .mt = MoveType::MOVE_CIRCLE,
          .live = true,
        };
        addProjectile(pp2, p2);
      }
      big[200] = pp2;
      big[220] = pp2;
      big[240] = pp2;

      addEnemy(ep, std::move(Enemy{
        .points = tmp,
        .shots = big,
        .size = 20,
        .health = 1,
        .speed = 5.0f,
        .spawntime = static_cast<float>(3),
        .sprite = tm.textures[8],
        .looped = false,
        }));





  }
  void load_stage1projectiles(ProjectilePool& pp);
}
