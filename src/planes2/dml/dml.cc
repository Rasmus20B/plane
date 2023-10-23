#include "dml.h"

#include "../input.h"
#include "scheduler.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cassert>
#include <cstring>
#include <iterator>
#include <raylib.h>
#include <thread>

namespace dml {

  bool VM::addBM(plane::BulletMgr bm) noexcept {
    uint16_t i = 0;
    while(i < this->bullets_mask.size()) {
      if(this->bullets_mask[i] == false) {
        bm.setOrigin(CURTASK.e->spatial.abspos + CURTASK.e->spatial.relpos);
        bm.shoot(bm.origin, p.spatial.pos);
        this->bullets[i] = bm;
        this->bullets_mask[i] = true;
        return true;
      }
      i++;
    }
    return false;
  }

  void VM::removeBM(const uint16_t id) noexcept {
    this->bullets_mask[id] = false;
  }

  void VM::load_script(const std::vector<uint8_t>&& progtext) noexcept {
    pgtext = std::move(progtext);

    // LOAD THE BACKGROUND
    bg = plane::tm.backgrounds[1];

  }

  [[nodiscard]]
  uint32_t VM::popInt(const uint32_t t_id) noexcept {
    uint32_t num = 0;
    if(CURTASK.sp[CURTASK.cur_co] <= 0) {
      return 0;
    }
    CURTASK.sp[CURTASK.cur_co] -= 4;
    num += (CURTASK.mem[CURTASK.sp[CURTASK.cur_co]+3] << 24) & 0xFF000000;
    num += (CURTASK.mem[CURTASK.sp[CURTASK.cur_co]+2] << 16) & 0x00FF0000;
    num += (CURTASK.mem[CURTASK.sp[CURTASK.cur_co]+1] << 8) & 0x0000FF00;
    num += (CURTASK.mem[CURTASK.sp[CURTASK.cur_co]] & 0x000000FF);
    return num;
  }

  [[nodiscard]]
  uint32_t VM::getIntFromArgument(const uint32_t t_id) noexcept {
    uint32_t num = 0;
    num += this->pgtext[CURTASK.pc[CURTASK.cur_co]+1] & 0x000000FF;
    num += (this->pgtext[CURTASK.pc[CURTASK.cur_co]+2] << 8) & 0x0000FF00;
    num += (this->pgtext[CURTASK.pc[CURTASK.cur_co]+3] << 16) & 0x00FF0000;
    num += (this->pgtext[CURTASK.pc[CURTASK.cur_co]+4] << 24) & 0xFF000000;
    return num;
  }
  
  void VM::init(const uint32_t t_id, const uint32_t start, const Display& d) noexcept {
    this->sch.tasks[t_id].pc[CURTASK.cur_co] = start;
    this->sch.tasks[t_id].waitctr[CURTASK.cur_co] = 0;
    this->sch.tasks[t_id].sp[CURTASK.cur_co] = 0;
    this->bullets_mask.resize(500);
    std::fill(this->bullets_mask.begin(), this->bullets_mask.end(), false);
    std::fill(this->sch.tasks[t_id].mem.begin(), this->sch.tasks[t_id].mem.end(), 0);
    this->bullets.resize(500);
    this->display = d;
  }

  void VM::pushInt(const uint32_t t_id, const uint32_t num) noexcept {
    CURTASK.mem[CURTASK.sp[CURTASK.cur_co]] = num & 0x000000FF;
    CURTASK.mem[CURTASK.sp[CURTASK.cur_co]+1] = (num & 0x0000FF00) >> 8;
    CURTASK.mem[CURTASK.sp[CURTASK.cur_co]+2] = (num & 0x00FF0000) >> 16;
    CURTASK.mem[CURTASK.sp[CURTASK.cur_co]+3] = (num & 0xFF000000) >> 24;
    CURTASK.sp[CURTASK.cur_co] += 4;
  }

  void VM::pushInt(const uint32_t t_id) noexcept {
    CURTASK.mem[CURTASK.sp[CURTASK.cur_co]] = this->pgtext[CURTASK.pc[CURTASK.cur_co]+1];
    CURTASK.mem[CURTASK.sp[CURTASK.cur_co] + 1] = this->pgtext[CURTASK.pc[CURTASK.cur_co]+2];
    CURTASK.mem[CURTASK.sp[CURTASK.cur_co] + 2] = this->pgtext[CURTASK.pc[CURTASK.cur_co]+3];
    CURTASK.mem[CURTASK.sp[CURTASK.cur_co] + 3] = this->pgtext[CURTASK.pc[CURTASK.cur_co]+4];
    CURTASK.sp[CURTASK.cur_co] += 4;
  }

  void VM::handle_bullets() noexcept {
    for(uint32_t id = 0; id < this->bullets_mask.size() - 1; ++id) {
      if(this->bullets_mask[id] == true) {
        this->bullets[id].update();
        this->bullets[id].draw();

        if(this->bullets[id].collision_check(
              {
              .x = p.spatial.pos.x + (p.sprite.width * 0.5f) - (p.in_sprite.width * 0.5f),
              .y = p.spatial.pos.y + (p.sprite.height * 0.5f)- (p.in_sprite.height * 0.5f),
              .width = (float)p.in_sprite.width ,
              .height = (float)p.in_sprite.height 
              })) {
          p.lives--;
          p.spatial.pos = {200, 800};
        }
  
        this->bullets[id].setOutOfBounds();

        bool ded = (std::adjacent_find(bullets[id].oobs.begin(), 
              bullets[id].oobs.end(), 
              std::not_equal_to<bool>()) == bullets[id].oobs.end());
        if(ded) {
          removeBM(id);
        }
      }
    }
  }

  void VM::fetch_execute() noexcept {
    return;
  }
  
  void VM::read_header() noexcept {
    constexpr std::array magic = { 0x7f, 0x44, 0x4d, 0x4c };
    std::cout << "LOADED SCRIPT SUCCESSFULLY\n";
    for(int i = 0; i < 4; ++i) {
      if(magic[i] != pgtext[i]) {
        std::cerr << "unknown file type\n";
        break;
      }
    }

    // Set pc to the entry point found in header file
    this->sch.tasks[0].pc[CURTASK.cur_co] += (this->pgtext[4]) & 0x000000FF;
    this->sch.tasks[0].pc[CURTASK.cur_co] += (this->pgtext[5] << 8) & 0x0000FF00;
    this->sch.tasks[0].pc[CURTASK.cur_co] += (this->pgtext[6] << 16) & 0x00FF0000;
    this->sch.tasks[0].pc[CURTASK.cur_co] += (this->pgtext[7] << 24) & 0xFF000000;

    pgtext.erase(pgtext.begin(), pgtext.begin() + 8);
  }

  void VM::render() noexcept{
    plane::handle_game_input(p.spatial,  p.shooting);
    BeginDrawing();
    DrawTextureEx(bg, {2.f * display.m_tile_size, 1.f * display.m_tile_size}, 0.0f, 1., WHITE);
    for(uint32_t i = 0; i < sch.tasks_mask.size(); ++i){
      if(sch.tasks_mask[i] == false || sch.tasks[i].base == false) continue;

      // Update Enemy positionsS
      if(!sch.tasks[i].waitctr[0] && sch.tasks[i].e->spatial.time) {
        // std::cout << "before: " << sch.tasks[i].e->spatial.abspos.x() << "\n";
        // std::cout << "absspeed: " << sch.tasks[i].e->spatial.absspeed << "\n";
        // std::cout << "absang: " << cos(sch.tasks[i].e->spatial.absang) << "\n";
        sch.tasks[i].e->spatial.abspos.vec.x += (sch.tasks[i].e->spatial.absspeed * cos(RAD(sch.tasks[i].e->spatial.absang)));
        sch.tasks[i].e->spatial.abspos.vec.y += (sch.tasks[i].e->spatial.absspeed * sin(RAD(sch.tasks[i].e->spatial.absang)));
        sch.tasks[i].e->spatial.relpos.vec.x += (sch.tasks[i].e->spatial.relspeed * cos(RAD(sch.tasks[i].e->spatial.relang)));
        sch.tasks[i].e->spatial.relpos.vec.y += (sch.tasks[i].e->spatial.relspeed * sin(RAD(sch.tasks[i].e->spatial.relang)));
        // std::cout << "after: " << sch.tasks[i].e->spatial.abspos.x() << "\n";
        sch.tasks[i].e->spatial.time--;
      }


      // Draw Enemies
      DrawTextureV(sch.tasks[i].e->sprite, {
          (sch.tasks[i].e->spatial.abspos.x() + sch.tasks[i].e->spatial.relpos.x()) - (sch.tasks[i].e->sprite.width * 0.5f), 
          (sch.tasks[i].e->spatial.abspos.y() + sch.tasks[i].e->spatial.relpos.y()) - (sch.tasks[i].e->sprite.height * 0.5f)
          }, sch.tasks[i].e->col);
    }

    // handle bullets
    DrawTextureV(p.sprite, 
        {p.spatial.pos.x , 
        p.spatial.pos.y },
        RAYWHITE);
    DrawTextureV( 
        p.in_sprite,
        {
        p.spatial.pos.x + (p.sprite.width * 0.5f) - (p.in_sprite.width * 0.5f),
        p.spatial.pos.y + (p.sprite.height * 0.5f) - (p.in_sprite.height * 0.5f)
        }, 
        RAYWHITE);
    handle_bullets();
    DrawFPS(0, 0);
    EndDrawing();
  }

  void VM::run(Display d) noexcept {

    double time_acc = 0.f;

    sch.set_ts_duration(16.67);
    sch.init();
    this->init(0, 0, d);

    read_header();

    while(!WindowShouldClose()) {
      ClearBackground(BLACK);

      uint32_t async_test = pgtext[CURTASK.pc[CURTASK.cur_co]];
      async_test <<= 8;
      async_test |= pgtext[CURTASK.pc[CURTASK.cur_co] + 1];

      if(async_test == static_cast<int>(OpCodes::CALLASYNC)
            || async_test == static_cast<int>(OpCodes::CALLASYNCID)) {

        // get the address to jump to 

        CURTASK.pc[CURTASK.cur_co] += 1;
        uint32_t addr = getIntFromArgument(sch.c_task);

        CURTASK.pc[CURTASK.cur_co] += sizeof(int) + 1;

        // Create a coroutine
        if(!sch.add_task(addr, sch.c_task, *CURTASK.e)) {
          std::cout << "Unable to add new task\n";
        }
        continue;
      }


      auto start_time = std::chrono::high_resolution_clock::now();
      if(sch.c_task == 0) {
        /* Every time we get back to task 0 (full revolution), do the rendering */
        render();
      }

      /* Check scheduler time limit */
      if(time_acc >= sch.cur_slice) {
        time_acc = 0.f;
        if(!sch.next_task()) return;
        continue;
      }

      /* If we are using a move that takes time 
         we don't fetch execute. We just count down. */
      if(CURTASK.e->spatial.time && CURTASK.base == true) {
        CURTASK.e->spatial.time--;
        if(!sch.next_task()) return;
        continue;
      }

      /* if we are waiting, we just count down */
      if(CURTASK.waitctr[CURTASK.cur_co]) {
        CURTASK.waitctr[CURTASK.cur_co] -= 1;
        if(!sch.next_task()) return;
        continue;
      }

      /* If we reach the end of our sub, then delete the thread */
      if(CURTASK.pc[CURTASK.cur_co] >= this->pgtext.size() ) {
        std::cout << "EOF\n";
        sch.del_task();
        if(!sch.next_task()) {
          return;
        }
        continue;
      }


      uint32_t opcode = pgtext[CURTASK.pc[CURTASK.cur_co]];
      opcode = opcode << 8;
      CURTASK.pc[CURTASK.cur_co]++;
      opcode |= pgtext[CURTASK.pc[CURTASK.cur_co]];
      std::cout << "THREAD " << std::hex << sch.c_task << " @" << CURTASK.pc[CURTASK.cur_co] << ": " << std::hex << opcode << "\n";
      OpCodes oc = static_cast<OpCodes>(opcode);
      switch(oc) {
        case OpCodes::NOP:
          CURTASK.pc[CURTASK.cur_co]++;
          break;
        case OpCodes::DELETE:
          sch.del_task();
          sch.next_task();
          if(sch.n_tasks == 0) return;
          break;
        case OpCodes::RETURN:
          CURTASK.pc[CURTASK.cur_co] = popInt(sch.c_task) ;
          break;
        case OpCodes::CALL:
          pushInt(sch.c_task, CURTASK.pc[CURTASK.cur_co] + sizeof(int) + 1);
          CURTASK.pc[CURTASK.cur_co] = getIntFromArgument(sch.c_task) ;
          break;
        case OpCodes::JMP:
          {
          uint32_t addr = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] = addr ;
          break;
          }
        case OpCodes::JMPEQ: 
          {
          uint32_t addr = getIntFromArgument(sch.c_task);
          uint32_t test = popInt(sch.c_task);
          if(test == 0) {
            CURTASK.pc[CURTASK.cur_co] = addr ;
          } else {
            CURTASK.pc[CURTASK.cur_co]+=sizeof(int) + 1;
          }
          break;
          }
        case OpCodes::JMPNEQ:
          {
          uint32_t addr = getIntFromArgument(sch.c_task);
          uint32_t test = popInt(sch.c_task);
          if(test > 0) {
            CURTASK.pc[CURTASK.cur_co] = addr;
          } else {
            CURTASK.pc[CURTASK.cur_co] += sizeof(int) + 1;
          }
          break;
          }
        case OpCodes::CALLASYNC:
          {
          pushInt(sch.c_task, CURTASK.pc[CURTASK.cur_co] + sizeof(int) + 1);
          CURTASK.pc[CURTASK.cur_co] = getIntFromArgument(sch.c_task);
          break;
          }
        case OpCodes::WAIT: {
          uint32_t time = getIntFromArgument(sch.c_task);
          CURTASK.waitctr[CURTASK.cur_co] = time;
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) + 1;
          break;
          };
        case OpCodes::PUSHI:
          {
          uint32_t var = getIntFromArgument(sch.c_task);
          pushInt(sch.c_task, var);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) + 1;
          break;
          }
        case OpCodes::SETI:
          {
          uint32_t var = getIntFromArgument(sch.c_task);
          uint32_t num = popInt(sch.c_task);
          CURTASK.vars[var] = num;
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) + 1;
          break;
          }
        case OpCodes::PUSHF:
          {
          break;
          }
        case OpCodes::SETF:
          {
          }
        case OpCodes::ADDI:
          {
          uint32_t res = 0;
          while(CURTASK.sp[CURTASK.cur_co] > 0) {
            res += popInt(sch.c_task);
          }

          pushInt(sch.c_task, res);
          CURTASK.pc[CURTASK.cur_co]++;
          break;
          }
        case OpCodes::ADDF: 
          {
          break;
          }

        /* ENEMY CREATION AND ANM SCRIPT MANAGEMENT */
        case OpCodes::ENMCREATE:
          {
          uint32_t addr = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) ;
          float x = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) ;
          float y = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) ;
          float health = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) ;
          float score = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) ;
          float item = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) + 1;

          if(!sch.add_task(addr, CURTASK.e->spatial.abspos.x() + x, CURTASK.e->spatial.abspos.y() + y, health, score, item)) {
            std::cout << "Unable to add new task\n";
          }
          break;
          }
        case OpCodes::ENMCREATEA:
          {
          CURTASK.pc[CURTASK.cur_co]+=sizeof(int) + 1;
          break;
          }
        case OpCodes::ANMSELECT:
          {
          uint32_t spr = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) + 1;
          break;
          }
        case OpCodes::ANMSETSPRITE:
          {
          uint32_t spr = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) ;
          uint32_t scr = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) + 1;
          CURTASK.e->sprite = plane::tm.actSprites[spr];
          CURTASK.e->col = RAYWHITE;
          break;
          }
        case OpCodes::ANMSETMAIN:
          {
          uint32_t slot = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) ;
          uint32_t scr = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) + 1;
          break;
          }
        case OpCodes::ENMDELETE:
          sch.del_task();
          if(!sch.next_task()) return;
          break;

        case OpCodes::MOVEPOS:
          // movPos(x, y)
          { 
          uint32_t x = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) ;
          uint32_t y = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) + 1;
          CURTASK.e->spatial.abspos = {(float)x , (float)y};
          break;
          }
        case OpCodes::MOVEPOSTIME:
          {
          // movPosTime(time, type, x, y)
          uint32_t time = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) ;
          uint32_t type = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) ;
          uint32_t x = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) ;
          uint32_t y = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) + 1;

          // v = d / t
          float distance = std::sqrt(std::pow(x - CURTASK.e->spatial.abspos.x(), 2) + std::pow(y - CURTASK.e->spatial.abspos.y(), 2));
          CURTASK.e->spatial.absspeed = distance / (float)time;
          std::cout << std::dec << CURTASK.e->spatial.abspos.x() << ", " << CURTASK.e->spatial.abspos.y() << " to " << x << ", " << y << "\n";
          std::cout << "distance: " << distance << " in " << time << " frames" << " @ " << CURTASK.e->spatial.absspeed << "\n";
          float atest = Vec2{float(x), float(y)}.dot(CURTASK.e->spatial.abspos) / (Vec2{float(x), float(y)}.magnitude() * CURTASK.e->spatial.abspos.magnitude());
          std::cout << "CALC: " << atest << "\n";
          // CURTASK.e->spatial.absang =  (acos(RAD(atest)) * 180) / PI;
          CURTASK.e->spatial.absang =  (atan(atest)) * 180 / PI;
          std::cout << CURTASK.e->spatial.absang << "\n";
          CURTASK.e->spatial.time = time;
          break;
          }
        case OpCodes::MOVEPOSRELTIME:
          break;

        case OpCodes::MOVEVEL:
          {
          uint32_t ang = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += (sizeof(int));
          uint32_t spd = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += (sizeof(int)) + 1;
          CURTASK.e->spatial.absang = ang;
          CURTASK.e->spatial.absspeed = spd;
          break;
          }
        case OpCodes::MOVEVELTIME:
          {
          uint32_t time = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += (sizeof(int));
          uint32_t mode = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += (sizeof(int));
          float ang = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += (sizeof(int));
          float spd = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += (sizeof(int)) + 1;
          CURTASK.e->spatial.absang = ang;
          CURTASK.e->spatial.absspeed = spd;
          CURTASK.e->spatial.time = time;
          CURTASK.e->spatial.movement = static_cast<plane::enmMoveType>(mode);
          break;
          }
        case OpCodes::MOVEVELREL:
          {
          uint32_t ang = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += (sizeof(int));
          uint32_t spd = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += (sizeof(int)) + 1;
          CURTASK.e->spatial.relang = ang;
          CURTASK.e->spatial.relspeed = spd;
          break;
          }

        /* Bullet Management */
        case OpCodes::ETNEW:
          {
          uint32_t idx = getIntFromArgument(sch.c_task);
          plane::BulletMgr b {
            .mode = plane::BulletFlag::AIMED,
          };
          b.setType(plane::BulletSprite::BLADE_01);
          CURTASK.bm[idx] = b;
          std::fill(CURTASK.bm[idx].oobs.begin(), CURTASK.bm[idx].oobs.end(), true);
          std::fill(CURTASK.bm[idx].positions.begin(), CURTASK.bm[idx].positions.end(), Vec2{0, 0});
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) + 1;
          break;
          }
        case OpCodes::ETON:
          {
          uint32_t idx = getIntFromArgument(sch.c_task);
          addBM(CURTASK.bm[idx]);
          CURTASK.pc[CURTASK.cur_co]+= sizeof(int) + 1;
          break;
          }
        case OpCodes::ETSPRITE:
          {
          uint32_t idx = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) ;
          uint32_t sprite = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) ;
          uint32_t colour = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) + 1;
          CURTASK.bm[idx].setType(static_cast<plane::BulletSprite>(sprite));
          break;
          }
        case OpCodes::ETOFFSET:
          break;
        case OpCodes::ETANGLE:
          {
          uint32_t idx = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) ;
          uint32_t ang1 = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) ;
          uint32_t ang2 = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) + 1;
          CURTASK.bm[idx].setAngle(ang1, ang2);
          break;
          }
        case OpCodes::ETSPEED:
          {
          uint32_t idx = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) ;
          uint32_t sp1 = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) ;
          uint32_t sp2 = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) + 1;
          CURTASK.bm[idx].setSpeed(sp1, sp2);
          break;
          }
        case OpCodes::ETCOUNT:
          {
          uint32_t idx = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) ;
          uint32_t la = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) ;
          uint32_t co = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) + 1;
          CURTASK.bm[idx].setCount(la, co);
          break;
          }
        case OpCodes::ETAIM:
          {
          uint32_t idx = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) ;
          uint32_t aim = getIntFromArgument(sch.c_task);
          CURTASK.pc[CURTASK.cur_co] += sizeof(int) + 1 ;
          CURTASK.bm[idx].setAim(static_cast<plane::BulletFlag>(aim));
          break;
          }
        default:
          std::cout << "OPCODE NOT IMPLEMENTED: " << opcode << "\n";
          return;
      }
      auto dur_taken = std::chrono::high_resolution_clock::now();
      std::chrono::duration<float, std::milli> acc = dur_taken - start_time;
      time_acc += acc.count();
    }
  }

  void VM::stop() noexcept {
  }
}

