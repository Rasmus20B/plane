#include "dml.h"
#include <cstring>

namespace dml {

  void VM::load_script(const std::string&& progtext) {
    // Load text into the end of memory
    pgtext = std::move(progtext);
  }

  constexpr uint32_t VM::getIntFromStack(uint32_t t_id){
    uint32_t num = (tasks[t_id].mem[tasks[t_id].sp-4] << 24)
      | (tasks[t_id].mem[tasks[t_id].sp-3] << 16) 
      | (tasks[t_id].mem[tasks[t_id].sp-2] << 8) 
      | tasks[t_id].mem[tasks[t_id].sp-1];
    return num;
  }

  constexpr uint32_t VM::getIntFromArgument(uint32_t t_id) {
    uint32_t num = 0;
    num = this->pgtext[tasks[t_id].pc+1];
    num |= this->pgtext[tasks[t_id].pc+2];
    num |= this->pgtext[tasks[t_id].pc+3];
    num |= this->pgtext[tasks[t_id].pc+4];
    return num;
  }
  
  void VM::init() {
    tasks.reserve(256);
    tasks.push_back({
        .pc = 0,
        .sp = 0,
        .waitctr = 0,
        .mem = {}
        });
  }

  void VM::init(uint32_t t_id, uint32_t start) {
    tasks[t_id].pc = start;
    tasks[t_id].waitctr = 0;
    tasks[t_id].sp = 0;
  }

  constexpr void VM::loadIntToStack(uint32_t t_id, uint32_t num) {
    tasks[t_id].mem[tasks[t_id].sp++] = num << 24;
    tasks[t_id].mem[tasks[t_id].sp++] = num << 16;
    tasks[t_id].mem[tasks[t_id].sp++] = num << 8;
    tasks[t_id].mem[tasks[t_id].sp++] = num;
  }

  constexpr void VM::loadIntToStack(uint32_t t_id) {
    tasks[t_id].mem[tasks[t_id].sp++] = this->pgtext[tasks[t_id].pc+1];
    tasks[t_id].mem[tasks[t_id].sp++] = this->pgtext[tasks[t_id].pc+2];
    tasks[t_id].mem[tasks[t_id].sp++] = this->pgtext[tasks[t_id].pc+3];
    tasks[t_id].mem[tasks[t_id].sp++] = this->pgtext[tasks[t_id].pc+4];
  }

  void VM::run() {

    uint32_t t_id = 0;
    while(true) {
      if(this->power.test()) {
        return;
      }
      if(t_id == tasks.size()) {
        t_id = 0;
      }
      if(tasks[t_id].waitctr) {
        tasks[t_id].waitctr--;
        continue;
      }
      if(tasks[t_id].pc == this->pgtext.size()) {
        if(t_id == 0) {
          return;
        }
        tasks[t_id].pc = 0;
      }
      int opcode = pgtext[tasks[t_id].pc];
      opcode = opcode << 8;
      tasks[t_id].pc++;
      opcode |= pgtext[tasks[t_id].pc];
      OpCodes oc = static_cast<OpCodes>(opcode);
      std::cout << t_id << ": " << std::hex << opcode << "\n";
      switch(oc) {
        case OpCodes::NOP:
          // nop
          tasks[t_id].pc++;
        case OpCodes::DELETE:
          // delete execution
          tasks[t_id].pc++;
          break;
        case OpCodes::JMP:
          // jmp
          tasks[t_id].pc++;
          break;
        case OpCodes::JUMPEQ:
          tasks[t_id].pc++;
          break;
        case OpCodes::WAIT: {
          // wait
          uint32_t time = 0;
          time |= (pgtext[tasks[t_id].pc+4]);
          tasks[t_id].waitctr = time;
          tasks[t_id].pc += 5;
          t_id++;
          break;
          };
        case OpCodes::RETURN:
          // Return
          break;
        case OpCodes::PUSHI:
          // push i
          {
          loadIntToStack(t_id);
          tasks[t_id].pc += sizeof(int) + 1;
          break;
          }
        case OpCodes::SETI:
          // set i
          {
          uint32_t var = getIntFromArgument(t_id);
          uint32_t num = getIntFromStack(t_id);
          std::cout << "OVERWRITING VAR: " << var << " WITH " << num << "\n";
          this->vars[var] = num;
          tasks[t_id].pc += sizeof(int) + 1;
          tasks[t_id].sp -= 4;
          break;
          }
        case OpCodes::PUSHF:
          // push f
          {
          int num = getIntFromArgument(t_id);
          loadIntToStack(t_id);
          tasks[t_id].pc += sizeof(int) + 1;
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
          uint32_t o1 = getIntFromStack(t_id);
          tasks[t_id].sp -= 4;
          uint32_t o2 = getIntFromStack(t_id);
          tasks[t_id].sp -= 4;
          res = o1 + o2;

          loadIntToStack(t_id, res);
          tasks[t_id].pc += sizeof(int);
          break;
          }
        case OpCodes::ADDF: 
          {
          // add f 
          break;
          }


        case OpCodes::ENMCREATE:
          // enmCreate()
          // push an enemy to a vector
          // assign them a program counter
          task tnew;
          tasks.push_back(tnew);
          this->init(tasks.size() - 1, 2);
          tasks[t_id].pc++;
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
    }
  }

  void VM::stop() {
    this->power.test_and_set();
  }
}

