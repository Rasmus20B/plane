#include "dml/dml.h"
#include "plane.h"
#include "bullets.h"

#include <chrono>
#include <thread>

int main() {
  dml::VM cpu;
  
  std::string prog = { 0x01, 0x2C, 
                       0x00, 0x2A, 0x00, 0x00, 0x00, 0x03,
                       0x00, 0x2A, 0x00, 0x00, 0x00, 0x04, 
                       0x00, 0x32, 
                       0x00, 0x17, 0x00, 0x00, 0x00, 0x04, 
                       0x00, 0x2B, 0x00, 0x00, 0x00, 0x01,
                       0x00, 0x17, 0x00, 0x00, 0x00, 0x03};

  cpu.load_script(std::move(prog));

  cpu.init();
  std::jthread vmt ([&]() {
      cpu.run();
      });

  std::this_thread::sleep_for(std::chrono::duration(std::chrono::seconds(2)));
  cpu.stop();
  // plane::main_loop();
}
