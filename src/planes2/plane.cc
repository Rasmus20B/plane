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

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(config.screen_width, config.screen_height, "Plane");
    SetTargetFPS(60);

    int gamepad = 0;

    int exp = 0;

    TextureManager tm;
    tm.textures.reserve(10);
    initTextureManager(tm);

    ProjectilePool e_ps;
    projectilePoolInit(e_ps, 5000);
    ProjectilePool p_ps;
    projectilePoolInit(p_ps, 250);
    ProjectilePool d_ps;
    projectilePoolInit(p_ps, 40);
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

    std::cout << "LOADED STAGE 1\n";


    while(!WindowShouldClose()) {

      BeginDrawing();

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

        if(IsGamepadAvailable(gamepad)) {
          if(IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_MIDDLE)) {
            p.pos.x += GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_X);
            p.pos.y += GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_Y);
          }
        }

        if(IsKeyDown(KEY_LEFT_SHIFT)) {
          micro = true;
          p.speed = 3.0f;
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
          if(enemies.spawntimes[i] <= frame_count && enemies.health[i] > 0) {
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
                pos = {
                  enemies.space[i].position.vec.x + (float)(15 * cos((360.0f/s->second.size) * j)),
                  enemies.space[i].position.vec.y + (float)(15 * sin((360.0f/s->second.size) * j))
                };
              } else {
                pos = enemies.space[i].position;
              }
              Vec2 vel = s->second.spaces[j].velocity; 
              if(s->second.attrs[j] == ProjectileAttributes::PROJECTILE_ATTRIBUTES_AIMED) {
                vel = (Vec2(p.pos) - pos).norm() * s->second.spaces[j].speed;
              }
              addProjectile(e_ps, std::move(Projectile {
                  .position = pos,
                  .velocity = vel,
                  .angle = s->second.spaces[j].angle,
                  .sprite = s->second.sprite[j],
                  .mt = s->second.spaces[j].mt,
                  .live = true,
                  }));
            }
            enemies.shots[i].erase(frame_count);
          }
        }

        Rectangle p_hitbox = {
                  p.pos.x - (p.in_sprite.width / 2.f),
                  p.pos.y - (p.in_sprite.height / 2.f), 
                  static_cast<float>(p.in_sprite.width), 
                  static_cast<float>(p.in_sprite.height) 
        };


        for(int i = 0; i < e_ps.spaces.size(); ++i) {
          if(e_ps.spawntime[i] > frame_count && !e_ps.live[i]) continue;
          // For now just draw them increasing until they fall off the screen
          pMove(e_ps.spaces[i]);
          DrawTextureEx(e_ps.sprite[i], 
              { 
                e_ps.spaces[i].position.vec.x - (e_ps.sprite[i].width ) ,
                e_ps.spaces[i].position.vec.y - (e_ps.sprite[i].height ),
              },
              e_ps.spaces[i].angle, 1.0f, WHITE);

          Rectangle ps_hitbox = Rectangle{
                  e_ps.spaces[i].position.vec.x  - (e_ps.sprite[i].width ), 
                  e_ps.spaces[i].position.vec.y - (e_ps.sprite[i].height ),
                  static_cast<float>(e_ps.sprite[i].width ), 
                  static_cast<float>(e_ps.sprite[i].height )
                };
          if(!p.d_time && CheckCollisionRecsAngle(
                p_hitbox,
                0.0f,
                ps_hitbox,
                RAD(e_ps.spaces[i].angle)
                )) {
            p.lives--;
            p.d_time = 50;
            micro = false;

            addProjectile(d_ps, Projectile {
              .position = p.pos,
              .sprite = tm.textures[10],
              .live = true,
            });
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
              if(enemies.spawntimes[j] < frame_count) {
                Rectangle e_hitbox = {
                  enemies.space[j].position.vec.x - (enemies.sprite[j].width / 2.0f),
                  enemies.space[j].position.vec.y - (enemies.sprite[j].height / 2.0f),
                  static_cast<float>(enemies.sprite[j].width),
                  static_cast<float>(enemies.sprite[j].height),
                };
                if(enemies.health[j] && CheckCollisionRecsAngle(p_hitbox, 0, e_hitbox, 0)) {
                  enemies.health[j]--;
                  p_ps.live[i] = false;
                  if(!enemies.health[j]) {
                    addProjectile(d_ps, Projectile {
                      .position = enemies.space[j].position,
                      .sprite = tm.textures[10],
                      .live = true,
                      });
                  }
                }
              }
            }
            DrawTextureV(p_ps.sprite[i], {
                p_ps.spaces[i].position.vec.x - (p_ps.sprite[i].width / 2.0f),
                p_ps.spaces[i].position.vec.y - (p_ps.sprite[i].width / 2.0f),
                }, WHITE);
            p_ps.spaces[i].position.vec = { p.vec.x, p.vec.y - p_ps.spaces[i].velocity.vec.y};
          }
        }

        // Handle explosions
        for(int i = 0; i < d_ps.live.size(); ++i) {
          if(d_ps.sprite[i].width == d_ps.spaces[i].angle) {
            d_ps.live[i] = false;
          }
          if(d_ps.live[i]) {
            DrawTextureRec(tm.textures[10], Rectangle {d_ps.spaces[i].angle, 0, 128, 128},
                Vector2 {
                  d_ps.spaces[i].position.vec.x - (64),
                  d_ps.spaces[i].position.vec.y - (64),
                },
                WHITE);
            d_ps.spaces[i].angle += 128;
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
        exp += 128;
      EndDrawing();
      frame_count++;
    }
  }
}
