#ifndef _PATHFIND_GRID_HPP
#define _PATHFIND_GRID_HPP

#include "util.hpp"
#include <algorithm>
#include <functional>
#include <memory>
#include <queue>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace pathfind {
// A trait (C++20) used by the algorithm to check if movement is possible.
template <typename T> struct is_occupiable_impl {
  static bool check(const T &t) { return t.isOccupied(); }
};

// Specialization for pointer types.
template <typename T> struct is_occupiable_impl<T *> {
  static bool check(const T *t) { return t->isOccupied(); }
};

// Specialization for smart pointer types like std::shared_ptr.
template <typename T> struct is_occupiable_impl<std::shared_ptr<T>> {
  static bool check(const std::shared_ptr<T> &t) { return t->isOccupied(); }
};

template <typename T>
concept Occupiable = requires(T t) {
  { is_occupiable_impl<T>::check(t) } -> std::convertible_to<bool>;
};

// An interface used to vary the implementation for the data operated by A*.
template <typename T> class Grid {
public:
  virtual ~Grid() = default;
  // Checks if a position is occupied.
  virtual bool isOccupied(int x, int y) const = 0;
  virtual bool isValid(const Vec2i &position) const = 0; // Check if in bounds.
  virtual T at(int x, int y) const = 0;    // Get data at specific location.
  virtual Vec2i getDimensions() const = 0; // Dimensions of the underlying data.
};

template <typename T> class Grid2D : public Grid<T> {
private:
  std::vector<std::vector<T>> grid;

public:
  Grid2D(const std::vector<std::vector<T>> &grid) : grid(grid) {}

  // Checks if a position is occupied.
  bool isOccupied(int x, int y) const override {
    return !is_occupiable_impl<T>::check(grid[y][x]);
  }

  // Checks if a position provided is within the bounds of the grid.
  bool isValid(const Vec2i &position) const override {
    return position.x >= 0 && position.x < grid[0].size() && position.y >= 0 &&
           position.y < grid.size();
  }

  // Obtains the data at a specific location within the grid.
  T at(int x, int y) const override { return grid[y][x]; }

  // Obtains the dimensions of the grid.
  Vec2i getDimensions() const override {
    return Vec2i(grid[0].size(), grid.size());
  }
};

template <typename T> class LinearGrid : public Grid<T> {
private:
  std::vector<T> grid;
  int width;

public:
  LinearGrid(const std::vector<T> &grid, int width)
      : grid(grid), width(width) {}

  // Checks if a position is occupied.
  bool isOccupied(int x, int y) const override {
    return !is_occupiable_impl<T>::check(grid[y * width + x]);
  }

  // Checks if a position provided is within the bounds of the grid.
  bool isValid(const Vec2i &position) const override {
    int index = position.y * width + position.x;
    return position.x >= 0 && position.x < width && position.y >= 0 &&
           position.y < grid.size() / width && index < grid.size();
  }

  // Obtains the data at a specific location within the grid.
  T at(int x, int y) const override { return grid[y * width + x]; }

  // Obtains the dimensions of the grid.
  Vec2i getDimensions() const override {
    return Vec2i(width, grid.size() / width);
  }
};
} // namespace pathfind

#endif