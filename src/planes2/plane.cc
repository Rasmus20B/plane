#include "plane.h"
#include "projectile.h"
#include "virt_screen.h"

#include "../common/display.h"

#include <random>
#include <raylib.h>

namespace plane {


  float rand_f(float max) {
    return static_cast<float>(rand() / static_cast<float>(static_cast<float>(RAND_MAX)/max));
  }

  void main_loop() {
    srand(time(0));

    std::string level = "../assets/test.dml";
    std::ifstream instream(level, std::ios::in | std::ios::binary);
    std::vector<uint8_t> prog((std::istreambuf_iterator<char>(instream)), std::istreambuf_iterator<char>());

    SetConfigFlags(FLAG_MSAA_4X_HINT);

    Display display(1080, 768);
    gg.x = display.m_width / 10;
    gg.y = 0;
    // gg.r = (640.f / 3)  * 2; 20
    gg.r = display.m_tile_size * 24;
    gg.c = display.m_tile_size * 28;

    InitWindow(display.m_width, display.m_height, "Plane");
    SetTargetFPS(60);

    initTextureManager(tm);
    dml::VM cpu;

    cpu.load_script(std::move(prog));
    cpu.run(display);

    return;

    int gamepad = 0;
    int exp = 0;
    bool micro;
    uint8_t stage = 1;

    size_t frame_count = 0;

    Texture2D bg = tm.fx[1];
    Texture2D bgw = tm.fx[2];

    float bg_scroll = 0.0f;
    float bgw_scroll = 0.0f;
    
    bool pause = false;
    Player p;

    return;
    // auto timeline = s.timeline;
    // std::vector<BulletMgr> bs;
    // std::vector<Enm> etypes = s.e;
    // std::vector<Enm> es;
    // std::vector<BulletMgr> player_bullets;
    //
    // std::unordered_map<uint32_t, bool> liveBMs;
    // std::unordered_map<uint32_t, bool> livePBs;
    // std::unordered_map<uint32_t, bool> liveENs;
    // liveENs.reserve(timeline.size);
    //
    // while(!WindowShouldClose()) {
    //   ClearBackground(BLACK);
    //   BeginDrawing();
    //
    //   handle_game_input(p.spatial, p.shooting);
    //   auto p_hitbox = p.getPlayerHitbox();
    //   DrawTextureV(p.sprite, {p.spatial.pos.x , p.spatial.pos.y }, WHITE);
    //
    //   // Check Player Bullets
    //   for(auto b : player_bullets) {
    //     b.update();
    //     b.draw();
    //     for(int i = 0; i < es.size(); ++i) {
    //       Rectangle hitbox = {
    //         .x = es[i].spatial.abspos.x(),
    //         .y = es[i].spatial.abspos.y(),
    //         .width = static_cast<float>(es[i].sprite.width),
    //         .height = static_cast<float>(es[i].sprite.height)
    //       };
    //       if(b.collision_check(hitbox)) {
    //         es[i].attrs.health--;
    //       }
    //     }
    //   }
    //
    //   for(int i = 0; i < es.size(); ++i) {
    //     if(!liveENs[i]) continue;
    //     enmUpdatePos(es[i].spatial);
    //     es[i].draw();
    //
    //     auto s = es[i].shooting.shots.find(frame_count);
    //     if(s == es[i].shooting.shots.end()) continue;
    //     s->second.shoot(es[i].spatial.abspos, p.spatial.pos);
    //     bs.push_back(s->second);
    //     liveBMs[bs.size() - 1] = true;
    //   }
    //
    //   for(int i = 0; i < bs.size(); ++i) {
    //     if(!liveBMs[i]) continue;
    //     bs[i].update();
    //     bs[i].draw();
    //     if(bs[i].collision_check(p_hitbox)) {
    //       std::cout << "HIT*: " << frame_count << "\n";
    //     }
    //     bs[i].setOutOfBounds();
    //     bool ded = (std::adjacent_find(bs[i].oobs.begin(), 
    //           bs[i].oobs.end(), 
    //           std::not_equal_to<bool>()) == bs[i].oobs.end());
    //     if(ded) {
    //       liveBMs.erase(i);
    //     }
    //   }
    //
    //
    //   while(timeline.front()->first == frame_count) {
    //     auto spawns = timeline.pop_and_get()->second;
    //     for(auto i : spawns) {
    //       liveENs[es.size() ] = true;
    //       es.push_back(etypes[i]);
    //     }
    //   }
    //
    //   DrawRectangle(gg.x + gg.c, 0, config.screen_width - (gg.c + gg.x), config.screen_height, GRAY);
    //   DrawRectangle(0, 0, config.screen_width / 10, config.screen_height, GRAY);
    //   frame_count++;
    //
    //   EndDrawing();
      // bool micro = false; BeginDrawing();
      //   ClearBackground(GetColor(0x052c46ff));
      //
      //   float time = GetTime();
        // Controls
//         if(!p.d_time) {
//         if(IsKeyDown(KEY_UP)) {
//           p.pos.y -= p.speed;
//         }
//         if(IsKeyDown(KEY_DOWN)) {
//           p.pos.y += p.speed;
//         }
//         if(IsKeyDown(KEY_RIGHT)) {
//           p.pos.x += p.speed;
//         }
//         if(IsKeyDown(KEY_LEFT)) {
//           p.pos.x -= p.speed;
//         }
//
//
//         if(IsGamepadAvailable(gamepad)) {
//           if(IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_MIDDLE)) {
//             p.pos.x += GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_X);
//             p.pos.y += GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_Y);
//           }
//         }
//
//
//         if(IsKeyDown(KEY_SPACE)) {
//           if(time - p.last_shot > 0.10) {
//             Projectile tmp = {
//                 .position = p.pos,
//                 .old_position = p.pos,
//                 .velocity = {0, 20},
//                 .attr = {},
//                 .sprite = tm.fx[0],
//                 .live = true
//             };
//             addProjectile(p_ps, std::move(tmp));
//             p.last_shot = time;
//           }
//         }
//         }
//
//         bg_scroll += 0.7f;
//         bgw_scroll += 1.0f;
//
//         if(bg_scroll >= bg.height ){
//           bg_scroll = 0.0f;
//         } 
//         if(bgw_scroll >= bgw.height ) {
//           bgw_scroll = 0.0f;
//         } 
//
//         DrawTextureEx(bg, {0, bg_scroll}, 0.0f, 1.0f, WHITE);
//         DrawTextureEx(bg, {0, -(bg.height - bg_scroll)}, 0.0f, 1.0f, WHITE);
//
//         DrawTextureEx(bgw, {0, bgw_scroll}, 0.0f, 1.0f, WHITE);
//         DrawTextureEx(bgw, {0, -(bgw.height - bgw_scroll)}, 0.0f, 1.0f, WHITE);
//
//         if(!p.d_time) {
//           DrawTextureV(p.sprite, {p.pos.x - (p.sprite.width / 2.0f), p.pos.y - (p.sprite.height / 2.0f) }, WHITE);
//           if(micro) DrawTextureV(p.in_sprite, {p.pos.x - (p.in_sprite.width / 2.0f)  , p.pos.y - (p.in_sprite.height / 2.0f) }, WHITE);
//         } else {
//           if(!p.lives) return;
//           p.d_time--;
//         }
//
//         // Handle enemies
//         for(int i = 0; i < enemies.space.size() ; ++i) {
//           if(enemies.spawntimes[i] <= frame_count && enemies.health[i] > 0) {
//             DrawTextureEx(enemies.sprite[i], {
//                 enemies.space[i].position.vec.x - (enemies.sprite[i].width / 2.0f),
//                 enemies.space[i].position.vec.y - (enemies.sprite[i].height / 2.0f),
//                 }, 0.0f, 1.0f, WHITE);
//             if(enemies.space[i].current_t >= enemies.space[i].points.size() - 1 ) {
//               if(enemies.looped[i]) {
//                 enemies.space[i].current_t = 0;
//               } else {
//                 goto shooting;
//               }
//             }
//             enemies.space[i].position = enemies.space[i].points[enemies.space[i].current_t++];
//           } else continue;
// shooting:
//           auto s = enemies.shots[i].find(frame_count);
//           if(s != enemies.shots[i].end()) {
//             for(int j = 0; j < s->second.live.size(); ++j) {
//               Vec2 pos;
//               if(s->second.spaces[j].mt == MoveType::MOVE_CIRCLE) {
//                 pos = {
//                   enemies.space[i].position.vec.x + (float)(15 * cos((360.0f/s->second.size) * j)),
//                   enemies.space[i].position.vec.y + (float)(15 * sin((360.0f/s->second.size) * j))
//                 };
//               } else {
//                 pos = enemies.space[i].position;
//               }
//               Vec2 vel = s->second.spaces[j].velocity; 
//               float angle = 0;
//               if(s->second.attrs[j] == ProjectileAttributes::PROJECTILE_ATTRIBUTES_AIMED) {
//                 vel = (Vec2(p.pos) - pos).norm() * s->second.spaces[j].speed;
//                 angle = vel.face_velocity();
//               } else {
//                 angle = s->second.spaces[j].angle;
//               }
//               addProjectile(e_ps, std::move(Projectile {
//                   .position = pos,
//                   .velocity = vel,
//                   .angle = angle,
//                   .angle_inc = s->second.spaces[j].angle_inc,
//                   .sprite = s->second.sprite[j],
//                   .shape = s->second.spaces[j].shape,
//                   .mt = s->second.spaces[j].mt,
//                   .live = true,
//                   }));
//             }
//             enemies.shots[i].erase(frame_count);
//           }
//         }
//
//         for(int i = 0; i < e_ps.spaces.size(); ++i) {
//           if(e_ps.spawntime[i] > frame_count || !e_ps.live[i]) continue;
//           // For now just draw them increasing until they fall off the screen
//           pMove(e_ps.spaces[i]);
//           Rectangle ps_hitbox = Rectangle{
//                   e_ps.spaces[i].position.vec.x, 
//                   e_ps.spaces[i].position.vec.y, 
//                   static_cast<float>(e_ps.sprite[i].width ), 
//                   static_cast<float>(e_ps.sprite[i].height )
//                 };
//
// #ifdef DRAW_HITBOX
//           DrawRectanglePro(ps_hitbox, 
//               {(float)e_ps.sprite[i].width / 2 , (float)e_ps.sprite[i].height / 2 },
//               e_ps.spaces[i].angle,
//               GetColor(0x00ff00ff));
//
//           DrawRectanglePro(p_hitbox, 
//               {(float)p.sprite.width / 2, (float)p.sprite.height / 2}, 
//               0.0f, 
//               GetColor(0x00ff00ff));
// #endif
//           DrawTexturePro(
//               e_ps.sprite[i], 
//               { 
//                 0,
//                 0,
//                 (float)e_ps.sprite[i].width,
//                 (float)e_ps.sprite[i].height
//               },
//               ps_hitbox,
//               {(float)e_ps.sprite[i].width / 2 , (float)e_ps.sprite[i].height / 2 },
//               (e_ps.spaces[i].angle), WHITE);
//
//
//           if(!p.d_time) {
//             bool hit;
//             switch(e_ps.spaces[i].shape) {
//               case ProjectileShape::PS_RECT:
//                 hit = CheckCollisionRecsAngle(
//                       p_hitbox,
//                       RAD(0.0f),
//                       ps_hitbox,
//                       RAD(e_ps.spaces[i].angle)
//                       );
//                 break;
//               case ProjectileShape::PS_CIRC:
//                 hit = CheckCollisionCircleRec({
//                     e_ps.spaces[i].position.vec.x ,
//                     e_ps.spaces[i].position.vec.y ,
//                     },
//                     e_ps.sprite[i].width / 2.0f, p_hitbox);
//                 break;
//               default:
//                 break;
//             }
//             if(hit) {
//               p.lives--;
//               p.d_time = 50;
//               micro = false;
//
//               addProjectile(d_ps, Projectile {
//                 .position = p.pos,
//                 // .sprite = tm.textures[10],
//                 .live = true,
//               });
//
//               for(int i = 0; i < e_ps.live.size(); ++i) {
//                 e_ps.live[i] = false;
//               }
//               p.pos = {config.screen_width/2,(config.screen_height/8) * 6};
//             }
//           }
//         }
//         for(int i = 0; i < p_ps.spaces.size(); ++i) {
//           // For now just draw them decreasing until they fall off the screen
//           if(p_ps.live[i]) {
//             auto p = p_ps.spaces[i].old_position = p_ps.spaces[i].position;
//             Rectangle ps_hitbox =  {
//               p_ps.spaces[i].position.vec.x,
//               p_ps.spaces[i].position.vec.y,
//               static_cast<float>(p_ps.sprite[i].width),
//               static_cast<float>(p_ps.sprite[i].height)
//             };
//             for(int j = 0; j < enemies.space.size(); ++j) {
//               if(enemies.spawntimes[j] < frame_count) {
//                 Rectangle e_hitbox = {
//                   enemies.space[j].position.vec.x, 
//                   enemies.space[j].position.vec.y,
//                   static_cast<float>(enemies.sprite[j].width),
//                   static_cast<float>(enemies.sprite[j].height),
//                 };
//                 if(enemies.health[j] && CheckCollisionRecsAngle(ps_hitbox, 0, e_hitbox, 0)) {
//                   enemies.health[j]--;
//                   p_ps.live[i] = false;
//                   if(!enemies.health[j]) {
//                     addProjectile(d_ps, Projectile {
//                       .position = enemies.space[j].position,
//                       // .sprite = tm.textures[10],
//                       .live = true,
//                       });
//                   }
//                 }
//               }
//             }
//             DrawTextureV(p_ps.sprite[i], {
//                 p_ps.spaces[i].position.vec.x - (p_ps.sprite[i].width / 2.0f),
//                 p_ps.spaces[i].position.vec.y - (p_ps.sprite[i].width / 2.0f)
//                 }, WHITE);
//             p_ps.spaces[i].position.vec = { p.vec.x, p.vec.y - p_ps.spaces[i].velocity.vec.y};
//           }
//         }
//
//         // Handle explosions
//         for(int i = 0; i < d_ps.live.size(); ++i) {
//           if(d_ps.sprite[i].width == d_ps.spaces[i].angle) {
//             d_ps.live[i] = false;
//           }
//           if(d_ps.live[i]) {
//             // DrawTextureRec(tm.textures[10], Rectangle {d_ps.spaces[i].angle, 0, 128, 128},
//                 // Vector2 {
//                 //   d_ps.spaces[i].position.vec.x - (64),
//                 //   d_ps.spaces[i].position.vec.y - (64),
//                 // },
//                 // WHITE);
//             d_ps.spaces[i].angle += 128;
//           }
//         }
//
//         // Handle player
//         //
//
//
//         DrawFPS(config.screen_width / 10, config.screen_height / 20);
//         DrawText(std::to_string(frame_count).c_str(), config.screen_width / 10, config.screen_height / 10, 20, WHITE);
//       EndDrawing();
//       frame_count++;
    //}
  }
}
