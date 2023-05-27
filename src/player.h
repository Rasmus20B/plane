
#include <raylib.h>

#include "config.h"


namespace plane {
struct Player {
  Vector2 pos{float(config.screen_width/2),float(config.screen_height/2)};
};
}
