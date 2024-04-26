#ifndef _WAVE_FUNCTION_COLLAPSE_HPP
#define _WAVE_FUNCTION_COLLAPSE_HPP

#include "ruleset.hpp"
#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <random>
#include <set>
#include <stack>
#include <thread>
#include <utility>
#include <vector>

namespace wfc {

template <typename T> class Cell {
public:
  std::vector<T> states; // All currently possible states.

  Cell(std::vector<T> states) : states(states) {}

  T state() { return states[0]; }               // State collapsed to.
  int entropy() { return states.size(); }       // Entropy of the cell.
  bool isCollapsed() { return entropy() == 1; } // Collapsed status.
  bool isInvalid() { return entropy() == 0; }   // Check if in invalid state.

  // Constrains the cell to abide by rules.
  bool constrain(std::vector<T> constraints) {
    // Prepare a vector to hold the intersection.
    std::vector<int> intersection;

    // Calculate the intersection.
    std::set_intersection(states.begin(), states.end(), constraints.begin(),
                          constraints.end(), std::back_inserter(intersection));

    if (states.size() != intersection.size()) {
      states = intersection;
      return true;
    }

    return false;
  }
};

template <typename T> using Wave = std::vector<std::vector<Cell<T>>>;

template <typename T> class WaveFunctionCollapse {
public:
  WaveFunctionCollapse(std::mt19937 &rng, int height, int width,
                       Ruleset<T> &rules, bool wrap)
      : rng(rng), wrap(wrap), rules(rules) {

    // Initialize the wave.
    std::vector<T> states = rules.allRules();
    wave = Wave<T>(height, std::vector<Cell<T>>(width, Cell(states)));
  }

  // Obtains the current status of the wave.
  Wave<T> getWave() const { return wave; }

  // Collapses the entire wave.
  void collapse() {
    while (!isCollapsed()) {
      next();
    }
  }

  // Processes the next iteration of the WFC algorithm.
  bool next() {
    std::optional<std::pair<int, int>> lowest_entropy = getMinEntropy();
    if (!lowest_entropy.has_value() ||
        wave[lowest_entropy->second][lowest_entropy->first].isCollapsed()) {
      done = true;
      return false;
    }

    int x = lowest_entropy->first;
    int y = lowest_entropy->second;

    collapseCell(x, y);
    to_proc.push({x, y});

    // Propagate the changes from the collapse to neighboring cells.
    while (!to_proc.empty()) {
      const auto [cx, cy] = to_proc.top();
      to_proc.pop();
      propagate(cx, cy);
    }

    return true;
  }

  // Checks if the wave is fully collapsed.
  bool isCollapsed() const { return done; }

private:
  std::mt19937 rng; // Used to have consistent randomization.
  bool wrap = false, done = false;
  Wave<T> wave;     // Wave / Map / Grid
  Ruleset<T> rules; // Rules and constraints for propagation.
  std::stack<std::pair<int, int>> to_proc; // (x, y) that need propagated.

  // Collapses cell (x, y).
  void collapseCell(int x, int y) {
    Cell<T> &cell = wave[y][x];
    if (cell.isCollapsed()) {
      return;
    }

    cell.states = {rules.pickRule(rng, cell.states)};
  }

  // Propagates the possible states to neighboring cells.
  void propagate(int x, int y) {
    Cell<T> &parent = wave[y][x];

    for (const auto &[nx, ny] : getNeighbors(x, y)) {
      Cell<T> &neighbor = wave[ny][nx];
      if (neighbor.isCollapsed()) {
        continue;
      }

      // Get the positioning relative to parent.
      int position = getPosition(x, y, nx, ny);

      std::set<int> all_states;
      for (const int state : parent.states) {
        // Accumulate all rules for the states.
        for (const int new_state : rules.getRule(state).atDirection(position)) {
          all_states.emplace(new_state);
        }
      }

      // Concert to vector and sort.
      std::vector<int> allowed(all_states.begin(), all_states.end());
      std::sort(allowed.begin(), allowed.end());

      // Constrain neighbor to states rules.
      if (neighbor.constrain(allowed)) {
        to_proc.push({nx, ny});
      }
    }
  }

  // Gets a random lowest entropy.
  std::optional<std::pair<int, int>> getMinEntropy() {
    int min_entropy = std::numeric_limits<int>::max();
    std::vector<std::pair<int, int>> candidates;

    // Check the entire wave for lowest entropy.
    for (int y = 0; y < wave.size(); y++) {
      for (int x = 0; x < wave[y].size(); x++) {
        auto &cell = wave[y][x];
        if (!cell.isCollapsed() && cell.entropy() < min_entropy) {
          min_entropy = cell.entropy();
          candidates = {{x, y}};
        } else if (cell.entropy() == min_entropy) {
          candidates.push_back({x, y});
        }
      }
    }

    // No more cells to collapse.
    if (candidates.empty()) {
      return std::nullopt;
    }

    // Select a random cell.
    std::uniform_int_distribution<int> dist(0, candidates.size() - 1);
    return candidates[dist(rng)];
  }

  // Obtains the neighbors, respecting the wave borders. Randomizes the results.
  std::vector<std::pair<int, int>> getNeighbors(int x, int y) {
    std::vector<std::pair<int, int>> neighbors;
    int height = wave.size();
    int width = wave[0].size();

    // North (-1, 0), East (0, +1), South (+1, 0), West (0, -1).
    std::array<std::pair<int, int>, 4> directions = {
        {{0, 1}, {1, 0}, {0, -1}, {-1, 0}}};

    for (const auto &[dx, dy] : directions) {
      int nx = x + dx;
      int ny = y + dy;

      if (wrap) {
        // Wrap around horizontally and vertically.
        nx = (nx + width) % width;
        ny = (ny + height) % height;
        neighbors.push_back({nx, ny});
      } else {
        // Ensure neighbors are within map boundaries.
        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
          neighbors.push_back({nx, ny});
        }
      }
    }

    // Shuffle neighbors to randomize processing order.
    std::shuffle(neighbors.begin(), neighbors.end(), rng);
    return neighbors;
  }

  // Get the position of (x2, y2) to (x1, y1)
  int getPosition(int x1, int y1, int x2, int y2) {
    int height = wave.size();
    int width = wave[0].size();

    // Calculate differences considering wrapping.
    int dx = (x2 - x1 + width) % width;
    int dy = (y2 - y1 + height) % height;

    // Normalize dx and dy to -1, 0, or 1 for easier comparison.
    if (dx > 0 && dx < width / 2)
      dx = 1; // Right
    else if (dx > width / 2)
      dx = -1; // Left (wrapped)

    if (dy > 0 && dy < height / 2)
      dy = 1; // Down
    else if (dy > height / 2)
      dy = -1; // Up (wrapped)

    // Return position index based on normalized dx and dy.
    if (dx == 1 && dy == 0)
      return 1; // East (right)
    if (dx == -1 && dy == 0)
      return 3; // West (left)
    if (dy == 1 && dx == 0)
      return 2; // South (down)
    if (dy == -1 && dx == 0)
      return 0; // North (up)

    // Default return for non-cardinal directions or error.
    return -1;
  }
};
} // namespace wfc

#endif