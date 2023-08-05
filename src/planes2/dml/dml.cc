#include "dml.h"
#include <chrono>
#include <cstring>
#include <iterator>
#include <raylib.h>

namespace dml {

  

  void VM::load_script(const std::string&& progtext) {
    // Load text into the end of memory
    pgtext = std::move(progtext);
  }

  constexpr uint32_t VM::getIntFromStack(uint32_t t_id){
    uint32_t num = 
        (CURTASK.mem[CURTASK.sp-4] << 24)
      | (CURTASK.mem[CURTASK.sp-3] << 16) 
      | (CURTASK.mem[CURTASK.sp-2] << 8) 
      |  CURTASK.mem[CURTASK.sp-1];
    CURTASK.sp -= 4;
    return num;
  }

  constexpr uint32_t VM::getIntFromArgument(uint32_t t_id) {
    uint32_t num = 0;
    num = this->pgtext[CURTASK.pc+1];
    num |= this->pgtext[CURTASK.pc+2];
    num |= this->pgtext[CURTASK.pc+3];
    num |= this->pgtext[CURTASK.pc+4];
    return num;
  }
  
  void VM::init() {
    tasks.reserve(256);
    tasks.push_back({
        .mem = {},
        .pc = 0,
        .sp = 0,
        .waitctr = 0,
        });
  }

  void VM::init(uint32_t t_id, uint32_t start) {
    CURTASK.pc = start;
    CURTASK.waitctr = 0;
    CURTASK.sp = 0;
  }

  constexpr void VM::loadIntToStack(uint32_t t_id, uint32_t num) {
    CURTASK.mem[CURTASK.sp+1] = num << 24;
    CURTASK.mem[CURTASK.sp+2] = num << 16;
    CURTASK.mem[CURTASK.sp+3] = num << 8;
    CURTASK.mem[CURTASK.sp+4] = num;
  }

  constexpr void VM::loadIntToStack(uint32_t t_id) {
    CURTASK.mem[CURTASK.sp+1] = this->pgtext[CURTASK.pc+1];
    CURTASK.mem[CURTASK.sp+2] = this->pgtext[CURTASK.pc+2];
    CURTASK.mem[CURTASK.sp+3] = this->pgtext[CURTASK.pc+3];
    CURTASK.mem[CURTASK.sp+4] = this->pgtext[CURTASK.pc+4];
  }

  void VM::run() {

    float time_acc = 0.f;

    sch.set_ts_duration(0.004);
    sch.init();
    while(true) {

      if(this->power.test()) {
        return;
      }

      if(time_acc >= sch.cur_slice) {
        time_acc = 0.f;
        sch.next_task();
      }

      if(CURTASK.waitctr) {
        CURTASK.waitctr -= 1;
        if(!sch.next_task()) {
          return;
        }
        continue;
      }

      if(CURTASK.pc >= this->pgtext.size() - 1) {

        if(!sch.next_task()) {
          return;
        }

        sch.del_task();
        sch.next_task();
        continue;
      }

      auto start_time = std::chrono::high_resolution_clock::now();
      int opcode = pgtext[CURTASK.pc];
      opcode = opcode << 8;
      CURTASK.pc++;
      opcode |= pgtext[CURTASK.pc];
      OpCodes oc = static_cast<OpCodes>(opcode);
      std::cout << sch.c_task << ": " << std::dec << CURTASK.pc << " = " << std::hex << opcode << "\n";
      switch(oc) {
        case OpCodes::NOP:
          // nop
          CURTASK.pc++;
        case OpCodes::DELETE:
          // delete execution
          CURTASK.pc++;
          break;
        case OpCodes::JMP:
          // jmp
          CURTASK.pc = getIntFromArgument(sch.c_task) ;
          break;
        case OpCodes::JUMPEQ:
          CURTASK.pc++;
          break;
        case OpCodes::WAIT: {
          // wait
          uint16_t time = getIntFromArgument(sch.c_task);
          CURTASK.waitctr = time;
          CURTASK.pc += sizeof(int) + 1;
          break;
          };
        case OpCodes::RETURN:
          // Return
          break;
        case OpCodes::PUSHI:
          // push i
          {
          loadIntToStack(sch.c_task);
          CURTASK.pc += sizeof(int) + 1;
          break;
          }
        case OpCodes::SETI:
          // set i
          {
          uint32_t var = getIntFromArgument(sch.c_task);
          uint32_t num = getIntFromStack(sch.c_task);
          CURTASK.vars[var] = num;
          CURTASK.pc += sizeof(int) + 1;
          CURTASK.sp -= 4;
          break;
          }
        case OpCodes::PUSHF:
          // push f
          {
          break;
          }
        case OpCodes::SETF:
          // set f
          {
          }
        case OpCodes::ADDI:
          {
          // add i 
          uint32_t res = 0;
          uint32_t o1 = getIntFromStack(sch.c_task);
          CURTASK.sp -= 4;
          uint32_t o2 = getIntFromStack(sch.c_task);
          CURTASK.sp -= 4;
          res = o1 + o2;

          loadIntToStack(sch.c_task, res);
          CURTASK.pc++;
          break;
          }
        case OpCodes::ADDF: 
          {
          // add f 
          break;
          }

        /* ENEMY CREATION AND ANM SCRIPT MANAGEMENT */
        case OpCodes::ENMCREATE:
          // enmCreate()
          {
          uint32_t addr = getIntFromArgument(sch.c_task);
          CURTASK.pc += sizeof(int) + 1;
          sch.add_task(addr);
          break;
          }

        case OpCodes::ENMDELETE:
          sch.del_task();
          if(!sch.next_task()) return;
          break;

        case OpCodes::MOVEPOS:
          // movPos(x, y)
          break;

        case OpCodes::MOVPOSTIME:
          // movPosTime(x, y, t)
          break;
        default:
          std::cout << "OPCODE NOT IMPLEMENTED\n";
          return;
      }
      auto dur_taken = std::chrono::high_resolution_clock::now();
      std::chrono::duration<float, std::milli> acc = dur_taken - start_time;
      time_acc += acc.count();
    }
  }

  void VM::stop() {
    this->power.test_and_set();
  }
}

