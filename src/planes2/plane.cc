#include "plane.h"
#include "projectile.h"

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

    std::unordered_map<int, Enemy> live_enemies;
    // Texture2D background = LoadTexture("../assets/city.png");
    float scroll = 0.0f;
    
    Player p;

    load_stage1enemies(enemies);

    while(!WindowShouldClose()) {

      float time = GetTime();
      // Controls
      //
      if(!p.d_time) {
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

      if(IsKeyDown(KEY_LEFT_SHIFT)) {
        p.speed = 5.0f;
      } else if(IsKeyReleased(KEY_LEFT_SHIFT)) {
        p.speed = 8.0f;
      }

      if(IsKeyDown(KEY_SPACE)) {
        if(time - p.last_shot > 0.10) {
          Projectile tmp = {
              .position = p.pos,
              .old_position = p.pos,
              .speed = 20,
              .radius = 10.0f,
              .colour = BLUE,
              .attr = {},
              .live = true
          };
          addProjectile(p_ps, std::move(tmp));
          p.last_shot = time;
        }
      }
      }
      scroll += 1.0f;
      // if(scroll >= background.height*2) scroll = background.height;
      BeginDrawing();
        // DrawTextureEx(background, {0, scroll}, 0.0f, 2.0f, WHITE);
        // DrawTextureEx(background, { 0, background.height * 2 + scroll}, 0.0f, 2.0f, WHITE);
        // Handle enemies
        for(int i = 0; i < enemies.space.size() ; ++i) {
          if(enemies.spawntime[i] <= time && enemies.health[i] > 0) {
            DrawCircleV(enemies.space[i].position.vec, enemies.space[i].size, enemies.colours[i]);
            if(enemies.space[i].current_t >= enemies.space[i].points.size() - 1 ) {
              if(enemies.looped[i]) {
                enemies.space[i].current_t = 0;
              } else {
                goto e_shooting;
              }
            }
            enemies.space[i].current_t++;
            enemies.space[i].position = enemies.space[i].points[enemies.space[i].current_t];
e_shooting:
            if(!enemies.last_shots[i]) {
              addProjectile(e_ps, std::move(Projectile{
                .position = enemies.space[i].position,
                .old_position = enemies.space[i].position,
                .speed = 7.0f,
                .radius = 10.0f,
                .colour = RAYWHITE,
                .live = true,
              }));
              enemies.last_shots[i] = 20;
            } else {
              enemies.last_shots[i]--;
            }
          }
        }


        for(int i = 0; i < e_ps.positions.size(); ++i) {
          // For now just draw them increasing until they fall off the screen
          auto tmp = e_ps.old_positions[i] = e_ps.positions[i];
          e_ps.positions[i].vec = { tmp.vec.x, tmp.vec.y + e_ps.speeds[i]};
          if(!p.d_time && CheckCollisionCircles(p.pos, p.in_size, e_ps.positions[i].vec, e_ps.radii[i])) {
            p.lives--;
            p.d_time = 50;
          }
          DrawCircleV(e_ps.positions[i].vec, e_ps.radii[i], e_ps.colours[i]);
        }

        for(int i = 0; i < p_ps.positions.size(); ++i) {
          // For now just draw them decreasing until they fall off the screen
          if(p_ps.live[i]) {
            auto p = p_ps.old_positions[i] = p_ps.positions[i];
            for(int j = 0; j < enemies.space.size(); ++j) {
              if(enemies.health[j] && CheckCollisionCircles(p_ps.positions[i].vec, p_ps.radii[i], enemies.space[j].position.vec, enemies.space[j].size)) {
                enemies.health[j]--;
                p_ps.live[i] = false;
              }
            }
            p_ps.positions[i].vec = { p.vec.x, p.vec.y - p_ps.speeds[i]};
            DrawCircleV(p_ps.positions[i].vec, p_ps.radii[i], p_ps.colours[i]);
          }
        }
        // Handle player
        if(!p.d_time) {
          DrawCircleV(p.pos, p.size, RED);
          DrawCircleV(p.pos, p.in_size, WHITE);
          DrawCircleV(p.pos, p.b_size, Color{255, 0, 0, 40});
        } else {
          if(!p.lives) return;
          p.d_time--;
        }

        DrawFPS(config.screen_width / 10, config.screen_height / 20);
        ClearBackground(GetColor(0x052c46ff));
      EndDrawing();
    }
  }
}
