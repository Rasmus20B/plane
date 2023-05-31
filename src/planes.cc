#include <vector> 

#include <raylib.h> 
#include "planes.h"

#include <stdlib.h> 

#include <iostream>

namespace plane {

  int main_loop() {

    Player p;
    InitWindow(config.screen_width, config.screen_height, "Plane");

    SetTargetFPS(60);

    int score = 0;

    float pos = 0;

    bool ded_flag;
    std::vector<Projectile> p_bullets;
    std::vector<Projectile> e_bullets;
    std::vector<Enemy<1>> enemies;
    p_bullets.reserve(40);
    e_bullets.reserve(200);
    enemies.reserve(100);
    float t_lastShot = GetTime();
    float t_lasteShot = GetTime();
    float t_laste = GetTime();

    enemies.push_back(Enemy<1>{
        .id = 0,
        .pos = {(config.screen_width/ 2), 0 }, 
        .mpat{ {{Vector2{1000, 40}, 0}, {Vector2{800, 70}, 0}, {Vector2{300, 100}, 0}, {Vector2{100, 40}, 0}, {Vector2{100, 80}, 0}, {Vector2{0, -100}, 0}}, 0 },
        .prog = 0,
        .spline_t = 0.01, 
        .shoot_t = 0.7f, 
        .last_shot = 0, 
        .speed = 8.0f, 
        .size = 20, 
        .lt = {0, 11}, 
        .dead = false,
        .points{}
        });
    enemies.push_back(Enemy<1>{
        .id = 1,
        .pos = {(config.screen_width/ 2), 0 }, 
        .mpat{ {{Vector2{1000, 40}, 0}, {Vector2{800, 70}, 0}, {Vector2{300, 100}, 0}, {Vector2{100, 40}, 0}, {Vector2{100, 80}, 0}, {Vector2{0, -100}, 0}}, 0 },
        .prog = 0,
        .spline_t = 0.01, 
        .shoot_t = 0.7f, 
        .last_shot = 0, 
        .speed = 8.0f, 
        .size = 20, 
        .lt = {0.20f, 11}, 
        .dead = false,
        .points{}
        });
    enemies.push_back(Enemy<1>{
        .id = 2,
        .pos = {(config.screen_width/ 2), 0 }, 
        .mpat{ {{Vector2{1000, 40}, 0}, {Vector2{800, 70}, 0}, {Vector2{300, 100}, 0}, {Vector2{100, 40}, 0}, {Vector2{100, 80}, 0}, {Vector2{0, -100}, 0}}, 0 },
        .prog = 0,
        .spline_t = 0.01, 
        .shoot_t = 0.7f, 
        .last_shot = 0, 
        .speed = 8.0f, 
        .size = 20, 
        .lt = {0.60, 11}, 
        .dead = false,
        .points{}
        });
    enemies.push_back(Enemy<1>{
        .id = 3,
        .pos = {(config.screen_width/ 2), 0 }, 
        .mpat{ {{Vector2{1000, 40}, 0}, {Vector2{800, 70}, 0}, {Vector2{300, 100}, 0}, {Vector2{100, 40}, 0}, {Vector2{100, 80}, 0}, {Vector2{0, -100}, 0}}, 0 },
        .prog = 0,
        .spline_t = 0.01, 
        .shoot_t = 0.2f, 
        .last_shot = 0, 
        .speed = 8.0f, 
        .size = 20, 
        .lt = {0.80, 11}, 
        .dead = false,
        .points{}
        });

    enemies.push_back(Enemy<1>{
        .id = 4,
        .pos = {(config.screen_width/ 2), 0 }, 
        .mpat{ {{Vector2{1000, 40}, 0}, {Vector2{800, 70}, 0}, {Vector2{300, 100}, 0}, {Vector2{100, 40}, 0}, {Vector2{100, 80}, 0}, {Vector2{0, -100}, 0}}, 0 },
        .prog = 0,
        .spline_t = 0.01, 
        .shoot_t = 0.7f, 
        .last_shot = 0, 
        .speed = 12.0f, 
        .size = 20, 
        .lt = {1, 11}, 
        .dead = false,
        .points{}
        });

    enemies.push_back(Enemy<1>{
        .id = 5,
        .pos = {(config.screen_width/ 2), 0 }, 
        .mpat{ {{Vector2{10, 400}, 0}, {Vector2{0, 100}, 0}, {Vector2{float(config.screen_width) + 20, 100}, 0}, {Vector2{600, 400}, 0}, {Vector2{800, 40}, 0}, {Vector2{1000, -200}, 0}, {Vector2{2000, -400}, 0} }, 0 },
        .prog = 0,
        .spline_t = 0.01, 
        .shoot_t = 0.7f, 
        .last_shot = 0, 
        .speed = 6.0f, 
        .size = 20, 
        .lt = {0, 11}, 
        .dead = false,
        .points{}
        });
    enemies.push_back(Enemy<1>{
        .id = 6,
        .pos = {(config.screen_width/ 2), 0 }, 
        .mpat{ {{Vector2{10, 400}, 0}, {Vector2{0, 100}, 0}, {Vector2{float(config.screen_width) + 20, 100}, 0}, {Vector2{600, 400}, 0}, {Vector2{800, 40}, 0}, {Vector2{1000, -100}, 0}, {Vector2{2000, -200}, 0} }, 0 },
        .prog = 0,
        .spline_t = 0.01, 
        .shoot_t = 0.7f, 
        .last_shot = 0, 
        .speed = 5.0f, 
        .size = 20, 
        .lt = {0.1, 11}, 
        .dead = false});
    enemies.push_back(Enemy<1>{
        .id = 7,
        .pos = {(config.screen_width/ 2), 0 }, 
        .mpat{ {{Vector2{10, 400}, 0}, {Vector2{0, 100}, 0}, {Vector2{float(config.screen_width) + 20, 100}, 0}, {Vector2{600, 400}, 0}, {Vector2{800, 40}, 0}, {Vector2{1000, -100}, 0}, {Vector2{2000, -200}, 0} }, 0 },
        .prog = 0,
        .spline_t = 0.01, 
        .shoot_t = 0.7f, 
        .last_shot = 0, 
        .speed = 5.0f, 
        .size = 20, 
        .lt = {0.20f, 11}, 
        .dead = false,
        .points{}
        });

    enemies.push_back(Enemy<1>{
        .id = 8,
        .pos = {(config.screen_width/ 2), 0 }, 
        .mpat{ {{Vector2{10, 400}, 0}, {Vector2{0, 100}, 0}, {Vector2{float(config.screen_width) + 20, 100}, 0}, {Vector2{600, 400}, 0}, {Vector2{800, 40}, 0}, {Vector2{1000, -100}, 0}, {Vector2{2000, -200}, 0} }, 0 },
        .prog = 0,
        .spline_t = 0.01, 
        .shoot_t = 0.7f, 
        .last_shot = 0, 
        .speed = 9.0f, 
        .size = 20, 
        .lt = {0.20f, 11}, 
        .dead = false,
        .points{}
        });

    enemies.push_back(Enemy<1>{
        .id = 9,
        .pos = {(config.screen_width/ 2), 0 }, 
        .mpat{ {{Vector2{10, 400}, 0}, {Vector2{0, 100}, 0}, {Vector2{float(config.screen_width) + 20, 100}, 0}, {Vector2{600, 400}, 0}, {Vector2{800, 40}, 0}, {Vector2{1000, -100}, 0}, {Vector2{2000, -200}, 0} }, 0 },
        .prog = 0,
        .spline_t = 0.01, 
        .shoot_t = 0.7f, 
        .last_shot = 0, 
        .speed = 5.0f, 
        .size = 20, 
        .lt = {0.40f, 11}, 
        .dead = false,
        .points{},
        });

    for(int j = 0; j < enemies.size(); j++) {
        for(int i = 0; i < enemies[j].mpat.pts.size(); i++) {
          enemies[j].mpat.len += (enemies[j].mpat.pts[i].len = enemies[j].mpat.SegmentLength(i, true));
        }
    }

      tmgr.task_queue.push({
        .func = &stage1,
        .state{}
      });
    while(!WindowShouldClose() && p.lives) {

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

      p.pos.x += GetFrameTime() * 100 * GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
      p.pos.y += GetFrameTime() * 100 * GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);

      if(IsKeyDown(KEY_SPACE)) {
        if(GetTime() - t_lastShot > 0.10) {
          Projectile tmp{ Vector2(p.pos), 10, 10.0f};
          p_bullets.push_back(tmp);
          t_lastShot = GetTime();
        }
      }
      if(IsKeyDown(KEY_LEFT_SHIFT)) {
        p.speed = 4;
        p.b_charging = true;
        if(p.b_size <= p.b_max_size) {
          p.b_size += 10.0f;
        }
        p.b_fire = false;
      } else {
        p.speed = 8;
        p.b_fire = true;
      }
      if(GetTime() - t_lasteShot > 1) {
        t_lasteShot = GetTime();
      }

      BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawCircleV(p.pos, p.size, RED);
        DrawCircleV(p.pos, p.in_size, WHITE);
        DrawCircleV(p.pos, p.b_size, Color{255, 0, 0, 40});

        for(auto &e : enemies) {
          if(!e.dead && e.lt.contains(GetTime()) ) {
            if(e.prog + e.speed >= e.mpat.len) e.prog = 0;
            e.spline_t = e.mpat.getNormalisedOffset(e.prog += e.speed);
            e.pos = e.mpat.getPoint(e.spline_t, true);
            SplinePt tmp = e.mpat.getGradient(e.spline_t, true);
            auto angle = Vec2Angle(e.pos.pos, tmp.pos);

            // Shooting at player
            if(GetTime() - e.last_shot > e.shoot_t) {
              e_bullets.push_back(Projectile{Vector2{e.pos.pos.x, e.pos.pos.y}, 10, 14, angle}); 
              e.last_shot = GetTime();
            }
            DrawCircleV(e.pos.pos, e.size, ORANGE);

            // Do the multishot stuff from player
            if(p.b_charging && !e.marked && CheckCollisionCircles(p.pos, p.b_size, e.pos.pos, e.size)) {
              p.marked.push_back(e.id);  
              e.marked = true;
            }
          }
        }
        if(p.b_fire && p.b_size) {
          for(auto &i : p.marked ) {
            enemies[i].marked = false;
            p_bullets.push_back(
                {p.pos, 20, 14, 0, true, true, i}
                );
          }
          p.b_size = 0;
          p.b_charging = false;
          p.marked.clear();
          
        }
        for(auto &i : p_bullets ) {
          if(i.live) {
            if(i.home) {

              // Make sure homing bullet is gone once target it dead
              if(enemies[i.target].dead) {
                i.live = false;
                continue;
              }
              auto dir = Vec2Norm(Vec2Sub(enemies[i.target].pos.pos, i.pos));
              i.pos.x = i.pos.x + dir.x * i.speed;
              i.pos.y = i.pos.y + dir.y * i.speed;
            } else {
              i.pos.y -= i.speed;
            }
            DrawCircleV(i.pos, i.size, BLUE);
            for(auto &e : enemies) {
              if(!e.dead  && CheckCollisionCircles(i.pos, i.size, e.pos.pos, e.size)) {
                e.dead = true;
                i.live = false;
                score++;
              }
            }
          }
        }
        for(auto &i : e_bullets) {
          if(i.live) {
            i.pos.y += i.speed + i.angle;
            i.pos.x += i.angle;
            DrawCircleV(i.pos, i.size, GREEN);
            if(CheckCollisionCircles(i.pos, i.size, p.pos, p.in_size)) {
              p.lives--;
              i.live = false;
            }
          }
        }
        DrawFPS((config.screen_width / 6) * 5, 20);
        DrawText(std::to_string(score).data(), (config.screen_width / 6) * 5, 40, 20, RED);
        DrawText(std::to_string(p.lives).data(), (config.screen_width / 6) * 5, 60, 20, RED);
      EndDrawing();
    }
  

    return 0;
  }
}
