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
    projectilePoolInit(e_ps, 5000);

    ProjectilePool p_ps;
    projectilePoolInit(p_ps, 250);

    EnemyPool enemies;
    enemyPoolInit(enemies, 40);

    std::unordered_map<int, Enemy> dead_enemies;
    
    Player p;

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

      float time = GetTime();
      // Controls
      if(IsKeyDown(KEY_UP)) {
        p.pos.y -= p.speed;
      }
      if(IsKeyDown(KEY_DOWN)) {
        p.pos.y += p.speed;
      }
      if(IsKeyDown(KEY_RIGHT)) {
        p.pos.x += p.speed;
      }
      if(IsKeyDown(KEY_LEFT)) {
        p.pos.x -= p.speed;
      }

      if(IsKeyDown(KEY_SPACE)) {
        if(time - p.last_shot > 0.10) {
          Projectile tmp = {
              .position = p.pos,
              .old_position = p.pos,
              .speed = 20,
              .radius = 10.0f,
              .colour = BLUE,
              .attr = {}
          };
          addProjectile(p_ps, std::move(tmp));
          p.last_shot = time;
        }
      }
      BeginDrawing();

        // Handle enemies
        for(int i = 0; i < enemies.movement_points.size() ; ++i) {
          if(enemies.spawntime[i] <= time && enemies.health[i] > 0) {
            DrawCircleV(enemies.positions[i].vec, enemies.sizes[i], enemies.colours[i]);
            if(enemies.current_points[i] >= enemies.movement_points[i].size() ) {
              if(enemies.looped[i]) {
                enemies.current_points[i] = 0;
              } else {
                continue;
              }
            } 
            auto pos = enemies.movement_points[i][enemies.current_points[i]++];
            enemies.positions[i].vec = pos.vec;
          }
        }


        for(int i = 0; i < e_ps.positions.size(); ++i) {
          // For now just draw them increasing until they fall off the screen
          auto p = e_ps.old_positions[i] = e_ps.positions[i];
          e_ps.positions[i].vec = { p.vec.x, p.vec.y + e_ps.speeds[i]};
          DrawCircleV(e_ps.positions[i].vec, e_ps.radii[i], e_ps.colours[i]);
        }

        for(int i = 0; i < p_ps.positions.size(); ++i) {
          // For now just draw them decreasing until they fall off the screen
          auto p = p_ps.old_positions[i] = p_ps.positions[i];
          for(int j = 0; j < enemies.positions.size(); ++j) {
            if(CheckCollisionCircles(p_ps.positions[i].vec, p_ps.radii[i], enemies.positions[j].vec, enemies.sizes[j])) {
              enemies.health[j] = 0;
            }
          }
          p_ps.positions[i].vec = { p.vec.x, p.vec.y - p_ps.speeds[i]};
          DrawCircleV(p_ps.positions[i].vec, p_ps.radii[i], p_ps.colours[i]);
        }
        // Handle player
        DrawCircleV(p.pos, p.size, RED);
        DrawCircleV(p.pos, p.in_size, WHITE);
        DrawCircleV(p.pos, p.b_size, Color{255, 0, 0, 40});


        DrawFPS(config.screen_width / 10, config.screen_height / 20);
        ClearBackground(RAYWHITE);
      EndDrawing();
    }
  }
}
