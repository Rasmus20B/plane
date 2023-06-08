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
    enemyPoolInit(enemies, 30);

    for(int i = 0; i < 3; ++i) {
      Enemy e {
        .points {
          Spline(
            std::vector<SplinePt>{
          {{config.screen_width - (50.0f * (float)i), 0}, 0},
          {{config.screen_width - (300.0f * (float)i), 200}, 0},
          {{config.screen_width - (300.0f * (float)i), 600}, 0},
          {{config.screen_width - (400.0f * (float)i), 500}, 0},
          }, 4).calc_points(0.01f, 10.0f, true)},
        .size = 30,
        .health = 1,
        .speed = 5.0f,
        .col = RED,
      };
      addEnemy(enemies, e);
      std::cout << enemies.movement_points[i][0].pos.x << ", " << enemies.movement_points[i][0].pos.y << ":" << enemies.movement_points[i][0].len << "\n";
    }


    while(!WindowShouldClose()) {

      for(int i = 0; i < 3; ++i) {
        if(enemies.current_points[i] == enemies.movement_points[i].size() - enemies.speeds[i]) enemies.current_points[i] = 0;
        DrawCircleV(enemies.positions[i].pos, enemies.sizes[i], enemies.colours[i]);
        enemies.current_points[i] += 1;
        auto pos = enemies.movement_points[i][enemies.current_points[i]];
        enemies.positions[i].pos.x = pos.pos.x;
        enemies.positions[i].pos.y = pos.pos.y;
      }

      BeginDrawing();
        ClearBackground(RAYWHITE);
      EndDrawing();
    }
  }
}
