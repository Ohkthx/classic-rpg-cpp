#ifndef _INPUT_HPP
#define _INPUT_HPP

#include "pathfinding/util.hpp"

class Input {
public:
  Vec2i movement_offset = Vec2i::ORIGIN();
  bool quit = false;

  // Checks if a keyboard action has happened.
  static Input check(bool);

private:
  // Creates an offset based on the character pressed.
  static Vec2i getMovementOffset(char);
};

#endif
