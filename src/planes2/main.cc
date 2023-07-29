#include "dml/dml.h"
#include "plane.h"
#include "bullets.h"

int main() {
  dml::VM cpu;
  std::string prog = { 0x00, 0x2A, 0x00, 0x00, 0x00, 0x03, 0x00, 0x2A, 0x00, 0x00, 0x00, 0x04, 0x00, 0x32, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04 };

  cpu.load_script(std::move(prog));

  cpu.init();
  cpu.run();

  cpu.stop();
  // plane::main_loop();
}
