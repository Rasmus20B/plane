#include <atomic>
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
    EnemyList e_list;
    e_list.list.reserve(100);
    e_mgr.data = &e_list;
    p_bullets.reserve(40);
    e_bullets.reserve(200);
    // e_list.reserve(100);
    float t_lastShot = GetTime();
    float t_lasteShot = GetTime();
    float t_laste = GetTime();

    e_mgr.task_queue.push({
        Stage1
    });


    while(!WindowShouldClose() && p.lives) {
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

      if(IsKeyPressed(KEY_ENTER)) {
        if(!e_mgr.paused.load()) {
          e_mgr.paused.store(true);
          e_mgr.paused.notify_all();
        } else {
          e_mgr.paused.store(false);
          e_mgr.paused.notify_all();
        }
      }
      p.pos.x += GetFrameTime() * 100 * GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
      p.pos.y += GetFrameTime() * 100 * GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);

      if(IsKeyDown(KEY_SPACE)) {
        if(GetTime() - t_lastShot > 0.10) {
          p_bullets.emplace_back( 
            Projectile {
              .pos = Vector2(p.pos),
              .speed = 20,
              .size = 10.0f
            } 
          );
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
      } else {
        if(GetTime() - p.d_time > 2) {
          p.d_time = 0;
          p.pos.x = config.screen_width / 2;
          p.pos.y = (config.screen_height / 6) * 5;
        }
      }
      if(GetTime() - t_lasteShot > 1) {
        t_lasteShot = GetTime();
      }

      BeginDrawing();
        ClearBackground(RAYWHITE);
        if(!p.d_time) {
          DrawCircleV(p.pos, p.size, RED);
          DrawCircleV(p.pos, p.in_size, WHITE);
          DrawCircleV(p.pos, p.b_size, Color{255, 0, 0, 40});
        }

        for(auto &e : e_mgr.data->list) {
          if(!e.dead && e.lt.contains(GetTime())) {
            // if(e.prog + e.speed >= e.mpat.len) e.prog = 0;
            // e.spline_t = e.mpat.getNormalisedOffset(e.prog += e.speed);
            // e.pos = e.mpat.getPoint(e.spline_t, true);
            // SplinePt tmp = e.mpat.getGradient(e.spline_t, true);
            // auto angle = Vec2Angle(e.pos.pos, tmp.pos);

            // Shooting at player
            if(GetTime() - e.last_shot > e.shoot_t) {
              e_bullets.push_back(Projectile{Vector2{e.pos.pos.x, e.pos.pos.y}, 10, 14, /* angle */}); 
              e.last_shot = GetTime();
            }
            DrawCircleV(e.pos.pos, e.size, ORANGE);
            e.draw->clear();
            e.draw->notify_all();
            // Do the multishot stuff from player
            if(p.b_charging && !e.marked && CheckCollisionCircles(p.pos, p.b_size, e.pos.pos, e.size)) {
              p.marked.push_back(e.id);  
              e.marked = true;
            }
          }
        }
        if(p.b_fire && p.b_size) {
          for(auto &i : p.marked ) {
            e_list.list[i].marked = false;
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
              if(e_list.list[i.target].dead) {
                i.live = false;
                continue;
              }
              auto dir = Vec2Norm(Vec2Sub(e_list.list[i.target].pos.pos, i.pos));
              i.pos.x = i.pos.x + dir.x * i.speed;
              i.pos.y = i.pos.y + dir.y * i.speed;
            } else {
              i.pos.y -= i.speed;
            }
            DrawCircleV(i.pos, i.size, BLUE);
            for(auto &e : e_list.list) {
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
            if(!p.d_time && CheckCollisionCircles(i.pos, i.size, p.pos, p.in_size)) {
              p.lives--;
              p.d_time = GetTime();
              i.live = false;
            }
          }
        }
        DrawFPS((config.screen_width / 6) * 5, 20);
        DrawText(std::to_string(score).data(), (config.screen_width / 6) * 5, 40, 20, RED);
        DrawText(std::to_string(p.lives).data(), (config.screen_width / 6) * 5, 60, 20, RED);

        if(e_mgr.paused.load()) {
          DrawText("Paused", (config.screen_width/2), 200, 20, RED);
        }
      EndDrawing();
    }
  

    return 0;
  }
}
