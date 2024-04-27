#ifndef _UI_TERMINAL_HPP
#define _UI_TERMINAL_HPP

#include "../map/map.hpp"

class Terminal {
public:
  static void clearScreen();
  void draw(const MapData &map);

private:
  int width;
  int height;

  void setSize();
};

#endif