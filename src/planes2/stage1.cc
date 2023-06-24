#include "stage1.h" 

namespace plane {
  void load_stage1enemies(TextureManager& tm,EnemyPool& ep) {
      std::unordered_map<int, ProjectilePool> shots;
      Projectile p1 = {
                .position = Vec2{0, 0},
                .old_position = {0, 0},
                .velocity = Vec2{3, 7.0f},
                .speed = 4.0f,
                .angle = 0,
                .attr = ProjectileAttributes::PROJECTILE_ATTRIBUTES_AIMED,
                .sprite = tm.textures[9],
                .mt = MoveType::MOVE_NORM,
                .live = true,
              };

      ProjectilePool pp1;
      projectilePoolInit(pp1, 4);
      addProjectile(pp1, p1);

      shots[420] = pp1;
      shots[510] = pp1;
      shots[620] = pp1;

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
        .health = 4,
        .speed = 5.0f,
        .spawntime = 100,
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
        .health = 4,
        .speed = 5.0f,
        .spawntime = 200,
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
        .health = 4,
        .speed = 5.0f,
        .spawntime = 200,
        .sprite = tm.textures[8],
        .looped = false,
        }));


      tmp = Spline( {
          {{config.screen_width / 2, 0}, 0},
          {{config.screen_width / 4, 300}, 0},
          {{config.screen_width / 5, 350}, 0},
          {{config.screen_width / 4, 400}, 0},
          }, 0).calc_points(0.01f, 5.0f, false);

      auto tmp2 = Spline( {
          {{config.screen_width / 2 * 2, 0}, 0},
          {{config.screen_width / 4 * 3, 300}, 0},
          {{config.screen_width / 5 * 4, 350}, 0},
          {{config.screen_width / 4 * 3, 400}, 0},
          }, 0).calc_points(0.01f, 5.0f, false);


      std::unordered_map<int, ProjectilePool> big;

      ProjectilePool pp2;
      projectilePoolInit(pp2, 20);
      for(int i = 0; i < 20; ++i) {
        Projectile p2 = {
          .velocity = {1, 1},
          .speed = 2,
          .angle = (90.f / 20) * i,
          .angle_inc = 0,
          .sprite = tm.textures[4],
          .mt = MoveType::MOVE_CIRCLE,
          .live = true,
        };
        addProjectile(pp2, p2);
      }

      ProjectilePool pp3;
      projectilePoolInit(pp3, 20);
      for(int i = 0; i < 20; ++i) {
        Projectile p2 = {
          .velocity = {1, 1},
          .speed = 2,
          .angle = (180.f / 20) * i,
          .angle_inc = 0,
          .sprite = tm.textures[4],
          .mt = MoveType::MOVE_CIRCLE,
          .live = true,
        };
        addProjectile(pp3, p2);
      }

      big[500] = pp2;
      big[520] = pp2;
      big[540] = pp2;
      big[700] = pp3;
      big[720] = pp3;
      big[740] = pp3;
      big[900] = pp3;
      big[920] = pp3;
      big[940] = pp3;

      addEnemy(ep, std::move(Enemy{
        .points = tmp,
        .shots = big,
        .size = 20,
        .health = 10,
        .speed = 5.0f,
        .spawntime = 300,
        .sprite = tm.textures[8],
        .looped = false,
        }));

      addEnemy(ep, std::move(Enemy{
        .points = tmp2,
        .shots = big,
        .size = 20,
        .health = 10,
        .speed = 5.0f,
        .spawntime = 300,
        .sprite = tm.textures[8],
        .looped = false,
        }));
  }
  void load_stage1projectiles(ProjectilePool& pp);
}
