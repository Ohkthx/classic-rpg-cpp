#ifndef _PATHFINDING_UTIL_HPP
#define _PATHFINDING_UTIL_HPP

#include <functional>
#include <type_traits>

struct Vec2i {
  int x, y;

  Vec2i(int x, int y) : x(x), y(y) {}

  Vec2i operator+(const Vec2i &other) const {
    return {x + other.x, y + other.y};
  }
  friend bool operator==(const Vec2i &a, const Vec2i &b) {
    return a.x == b.x && a.y == b.y;
  }

  friend bool operator!=(const Vec2i &a, const Vec2i &b) { return !(a == b); }

  // Orgin location.
  inline static Vec2i ORIGIN() { return {0, 0}; }

  bool isOrigin() { return this->x == ORIGIN().x && this->y == ORIGIN().y; }
};

namespace std {
template <> struct hash<Vec2i> {
  std::size_t operator()(const Vec2i &v) const {
    return std::hash<int>()(v.x * 73856093) ^ std::hash<int>()(v.y * 19349663);
  }
};
} // namespace std

#endif