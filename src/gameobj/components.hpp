#ifndef _GAME_OBJECT_COMPONENTS_HPP
#define _GAME_OBJECT_COMPONENTS_HPP

#include "../ecs/component.hpp"
#include <optional>

struct PathComponent : public ecs::Component {
  std::queue<Vec2i> path;
  PathComponent(std::queue<Vec2i> path) : path(path) {}

  // Obtains the next position, removing it from the queue.
  std::optional<Vec2i> next() {
    if (path.empty()) {
      return std::nullopt;
    }

    Vec2i pos = path.front();
    path.pop();
    return pos;
  }
};

struct PositionComponent : public ecs::Component, public Vec2i {
  using Vec2i::Vec2i; // Allows to treat it as if it is Vec2i.

  explicit PositionComponent(const Vec2i &vec) : Vec2i(vec) {}

  PositionComponent &operator=(const Vec2i &other) {
    Vec2i::operator=(other);
    return *this;
  }
};

#endif