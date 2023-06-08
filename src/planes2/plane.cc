#include "plane.h"

#include <random>

namespace plane {

  float rand_f(float max) {
    return static_cast<float>(rand() / static_cast<float>(static_cast<float>(RAND_MAX)/max));
  }

  void main_loop() {

    srand(time(0));

    InitWindow(config.screen_width, config.screen_height, "Plane");
    SetTargetFPS(60);

    ProjectilePool e_ps;
    projectilePoolInit(e_ps, 10000);

    ProjectilePool p_ps;
    projectilePoolInit(p_ps, 250);

    EnemyPool enemies;
    enemyPoolInit(enemies, 7000);

    for(int i = 0; i < 1; ++i) {
      Enemy e {
        .points {
          Spline(
            std::vector<SplinePt>{
          {{config.screen_width - 50.0f, 0}, 0},
          {{config.screen_width - 300.0f, 200}, 0},
          {{config.screen_width - 300.0f , 600}, 0},
          {{config.screen_width - 400.0f, 500}, 0},
          {{config.screen_width - 200.0f, 0}, 0},
          }, 0).calc_points(0.01f, 5.0f, false)},
        .size = 30,
        .health = 1,
        .speed = 5.0f,
        .col = RED,
        .looped = false,
        .spawntime = 1.0f
      };
      addEnemy(enemies, e);
    }

    while(!WindowShouldClose()) {
      
      BeginDrawing();
      for(int i = 0; i < enemies.movement_points.size() ; ++i) {
        if(enemies.spawntime[i] <= GetTime()) {
          DrawCircleV(enemies.positions[i].pos, enemies.sizes[i], enemies.colours[i]);
          if(enemies.current_points[i] >= enemies.movement_points[i].size() ) {
            if(enemies.looped[i]) {
              enemies.current_points[i] = 0;
            } else {
              continue;
            }
          } 
          auto pos = enemies.movement_points[i][enemies.current_points[i]++];
          enemies.positions[i].pos.x = pos.pos.x;
          enemies.positions[i].pos.y = pos.pos.y;
        }
      }

        DrawFPS(config.screen_width / 6, config.screen_height / 6);
        ClearBackground(RAYWHITE);
      EndDrawing();
    }
  }
}
