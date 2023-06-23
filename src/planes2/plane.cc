#include "plane.h"
#include "projectile.h"

#include <random>
#include <raylib.h>

namespace plane {

  float rand_f(float max) {
    return static_cast<float>(rand() / static_cast<float>(static_cast<float>(RAND_MAX)/max));
  }

  void main_loop() {
    srand(time(0));
    InitWindow(config.screen_width, config.screen_height, "Plane");
    SetTargetFPS(60);

    TextureManager tm;
    tm.textures.reserve(10);
    initTextureManager(tm);

    ProjectilePool e_ps;
    projectilePoolInit(e_ps, 5000);
    ProjectilePool p_ps;
    projectilePoolInit(p_ps, 250);
    EnemyPool enemies;
    enemyPoolInit(enemies, 40);
    std::unordered_map<int, Enemy> live_enemies;

    size_t frame_count = 0;

    Texture2D bg = tm.textures[6];
    Texture2D bgw = tm.textures[7];

    float bg_scroll = 0.0f;
    float bgw_scroll = 0.0f;
    
    bool micro = false;
    bool pause = false;
    Player p;

    load_stage1enemies(tm, enemies);


    while(!WindowShouldClose()) {

      static float rotation = 0;
      rotation += 0.5;

      float time = GetTime();
      // Controls

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
        micro = true;
        p.speed = 5.0f;
      } else if(IsKeyReleased(KEY_LEFT_SHIFT)) {
        micro = false;
        p.speed = 8.0f;
      }

      if(IsKeyDown(KEY_SPACE)) {
        if(time - p.last_shot > 0.10) {
          Projectile tmp = {
              .position = p.pos,
              .old_position = p.pos,
              .velocity = {0, 20},
              .attr = {},
              .sprite = tm.textures[1],
              .live = true
          };
          addProjectile(p_ps, std::move(tmp));
          p.last_shot = time;
        }
      }
      }
      // if(scroll >= background.height*2) scroll = background.height;

      BeginDrawing();

        bg_scroll += 0.7f;
        bgw_scroll += 1.0f;

        if(bg_scroll >= bg.height ){
          bg_scroll = 0.0f;
        } 
        if(bgw_scroll >= bgw.height ) {
          bgw_scroll = 0.0f;
        } 

        DrawTextureEx(bg, {0, bg_scroll}, 0.0f, 1.0f, WHITE);
        DrawTextureEx(bg, {0, -(bg.height - bg_scroll)}, 0.0f, 1.0f, WHITE);

        DrawTextureEx(bgw, {0, bgw_scroll}, 0.0f, 1.0f, WHITE);
        DrawTextureEx(bgw, {0, -(bgw.height - bgw_scroll)}, 0.0f, 1.0f, WHITE);

        // Handle enemies
        for(int i = 0; i < enemies.space.size() ; ++i) {
          if(enemies.spawntimes[i] <= time && enemies.health[i] > 0) {
            DrawTextureEx(enemies.sprite[i], {
                enemies.space[i].position.vec.x - (enemies.sprite[i].width / 2.0f),
                enemies.space[i].position.vec.y - (enemies.sprite[i].height / 2.0f),
                }, 0.0f, 1.0f, WHITE);
            if(enemies.space[i].current_t >= enemies.space[i].points.size() - 1 ) {
              if(enemies.looped[i]) {
                enemies.space[i].current_t = 0;
              } else {
                goto shooting;
              }
            }
            enemies.space[i].position = enemies.space[i].points[enemies.space[i].current_t++];
          } else continue;
shooting:
          auto s = enemies.shots[i].find(frame_count);
          if(s != enemies.shots[i].end()) {
            for(int j = 0; j < s->second.live.size(); ++j) {
              Vec2 pos;
              if(s->second.spaces[j].mt == MoveType::MOVE_CIRCLE) {
                // Vec2(centre.vec.x + (15 * cos((360.0f/n) * i)), centre.vec.y + (15 * sin((360.0f/n) * i)) );
                pos = {
                  enemies.space[i].position.vec.x + (float)(15 * cos((360.0f/s->second.size) * j)),
                  enemies.space[i].position.vec.y + (float)(15 * sin((360.0f/s->second.size) * j))
                };
              } else {
                pos = enemies.space[i].position;
              }
              addProjectile(e_ps, std::move(Projectile {
                  .position = pos,
                  .velocity = s->second.spaces[j].velocity,
                  .angle = s->second.spaces[j].angle,
                  .sprite = s->second.sprite[j],
                  .mt = s->second.spaces[j].mt,
                  .live = true,
                  }));
            }
            enemies.shots[i].erase(frame_count);
          }
        }

        for(int i = 0; i < e_ps.spaces.size(); ++i) {
          if(e_ps.spawntime[i] > time && !e_ps.live[i]) continue;
          // For now just draw them increasing until they fall off the screen
          pMove(e_ps.spaces[i]);
          DrawTextureEx(e_ps.sprite[i], 
              { 
                e_ps.spaces[i].position.vec.x ,
                e_ps.spaces[i].position.vec.y ,
              },
              e_ps.spaces[i].angle, 1.0f, WHITE);
          if(!p.d_time && CheckCollisionRecsAngle(Rectangle {
                p.pos.x - (p.in_sprite.width / 2.0f), p.pos.y - (p.in_sprite.height / 2.0f), 
                static_cast<float>(p.in_sprite.width), 
                static_cast<float>(p.in_sprite.height) }, 
                0.0f,
                Rectangle{e_ps.spaces[i].position.vec.x - (e_ps.sprite[i].width / 2.0f), 
                e_ps.spaces[i].position.vec.y - (e_ps.sprite[i].height / 2.0f),
                static_cast<float>(e_ps.sprite[i].width ), 
                static_cast<float>(e_ps.sprite[i].height )},
                e_ps.spaces[i].angle
                )) {
            p.lives--;
            p.d_time = 50;
            micro = false;
            p.pos = {config.screen_width/2,(config.screen_height/8) * 6};
          }
        }
        for(int i = 0; i < p_ps.spaces.size(); ++i) {
          // For now just draw them decreasing until they fall off the screen
          if(p_ps.live[i]) {
            auto p = p_ps.spaces[i].old_position = p_ps.spaces[i].position;
            Rectangle p_hitbox =  {
              p_ps.spaces[i].position.vec.x - (p_ps.sprite[i].width / 2.0f),
              p_ps.spaces[i].position.vec.y - (p_ps.sprite[i].height / 2.0f),
              static_cast<float>(p_ps.sprite[i].width),
              static_cast<float>(p_ps.sprite[i].height)
            };
            for(int j = 0; j < enemies.space.size(); ++j) {
              Rectangle e_hitbox = {
                enemies.space[j].position.vec.x - (enemies.sprite[j].width / 2.0f),
                enemies.space[j].position.vec.y - (enemies.sprite[j].height / 2.0f),
                static_cast<float>(enemies.sprite[j].width),
                static_cast<float>(enemies.sprite[j].height),
              };
              if(enemies.health[j] && CheckCollisionRecsAngle(p_hitbox, 0, e_hitbox, 0)) {
                enemies.health[j]--;
                p_ps.live[i] = false;
              }
            }
            DrawTextureV(p_ps.sprite[i], {
                p_ps.spaces[i].position.vec.x - (p_ps.sprite[i].width / 2.0f),
                p_ps.spaces[i].position.vec.y - (p_ps.sprite[i].width / 2.0f),
                }, WHITE);
            p_ps.spaces[i].position.vec = { p.vec.x, p.vec.y - p_ps.spaces[i].velocity.vec.y};
          }
        }

        // Handle player
        //
        if(!p.d_time) {
          DrawTextureV(p.sprite, {p.pos.x - (p.sprite.width / 2.0f), p.pos.y - (p.sprite.height / 2.0f) }, WHITE);
          if(micro) DrawTextureV(p.in_sprite, {p.pos.x - (p.in_sprite.width / 2.0f)  , p.pos.y - (p.in_sprite.height / 2.0f) }, WHITE);
        } else {
          if(!p.lives) return;
          p.d_time--;
        }


        DrawFPS(config.screen_width / 10, config.screen_height / 20);
        DrawText(std::to_string(frame_count).c_str(), config.screen_width / 10, config.screen_height / 10, 20, WHITE);
        ClearBackground(GetColor(0x052c46ff));
      EndDrawing();
      frame_count++;
    }
  }
}
