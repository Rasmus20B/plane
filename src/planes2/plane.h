#include <vector>

#include <raylib.h>

namespace plane {

  struct Projectiles {
    std::vector<float> positions;
    std::vector<float> old_position;
    std::vector<float> size;
    std::vector<Vector4> colours;
  };

  void main_loop();

}
