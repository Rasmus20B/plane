#include <cstdint>

struct Display { 
  Display(uint16_t width, uint16_t height) {
    m_width = width;
    m_height = height;
    m_tile_size = width / 40;

  }
  uint16_t m_width;
  uint16_t m_height;
  uint16_t m_tile_size;
};
