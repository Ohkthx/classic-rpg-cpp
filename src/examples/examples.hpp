#ifndef _ALGO_EXAMPLES_HPP
#define _ALGO_EXAMPLES_HPP

#include "../generation/terrain/wfc.hpp"
#include "../pathfinding/astar.hpp"
#include "../tick.hpp"
#include <iostream>

// Demonstrates how to initialize and visualize the WFC algorithm.
wfc::Wave<int> wfcExample(int height, int width, bool display, bool animate);

// Demonstrates how to initialize and visualize the Tileset w/ WFC algorithm.
void tilesetExample(wfc::Wave<int> &wave);

// Demonstrates how to use the tick mechanic.
void tickExample(std::size_t tick_interval) {
  // Calculate interval for target frame rate.
  TickController tick_controller(tick_interval);

  while (tick_controller.getTick() < 100) {
    std::cout << std::format("Tick: {}\n", tick_controller.getTick());
    tick_controller.tick();
  }

  std::cout << "All ticks complete." << std::endl;
}

// Demonstrates the A* Pathfinding.
void astarExample(int height, int width, bool allow_diagonal) {
  // Used to represent each cell, `isOccupied()` is required for A*.
  struct Cell {
    bool state = false;
    bool isOccupied() const { return state; }
  };

  // Create and pathfind.
  std::vector<std::vector<Cell>> grid;
  grid.resize(height, std::vector<Cell>(width));

  pathfind::AStar<Cell> astar(grid, allow_diagonal, false);
  std::vector<Vec2i> path = astar.findPath({0, 0}, {9, 9});

  // Worst case scenario.
  if (path.empty()) {
    std::cout << "No path found." << std::endl;
  } else {
    for (const auto &p : path) {
      std::cout << "(" << p.x << ", " << p.y << ")  ";
    }
  }
  std::cout << std::endl;

  // Create a display grid.
  std::vector<std::vector<char>> display(
      astar.getHeight(), std::vector<char>(astar.getWidth(), '.'));

  // Mark the path on the display grid.
  if (!path.empty()) {
    for (auto node : path) {
      display[node.y][node.x] = '*';
    }
  }

  for (int y = 0; y < astar.getHeight(); ++y) {
    for (int x = 0; x < astar.getWidth(); ++x) {
      std::cout << display[y][x] << " ";
    }
    std::cout << "\n";
  }
}

#endif