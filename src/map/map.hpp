#ifndef _MAP_DATA_HPP
#define _MAP_DATA_HPP

#include "../generation/terrain/wfc.hpp"
#include "../pathfinding/astar.hpp"
#include "../pathfinding/util.hpp"
#include "../tileset.hpp"
#include "tile.hpp"
#include <memory>
#include <queue>
#include <random>
#include <vector>

class MapData {
public:
  std::mt19937 rng;
  std::vector<std::shared_ptr<Tile>> data;
  int _width, _height;

  MapData(std::mt19937 &rng, int height, int width,
          wfc::Ruleset<int> rules = wfc::Ruleset<int>::DefaultRules())
      : rng(rng) {
    // Initialize and collapse the map using WFC.
    wfc::WaveFunctionCollapse wfc(rng, height, width, rules, false);
    wfc.collapse();

    // Apply the wave to the map.
    applyWave(wfc.getWave());
  }

  int width() const { return _width; }
  int height() const { return _height; }

  // Obtains a Tile as a position (x, y) in the map.
  std::shared_ptr<Tile> at(int x, int y) const {
    if (inBounds(x, y)) {
      return data[y * _width + x];
    }

    throw std::out_of_range("Index out of bounds for map data.");
  }

  // Check if the given position is within bounds.
  bool inBounds(int x, int y) const {
    return x >= 0 && x < _width && y >= 0 && y < _height;
  }

  // Obtains a random position that is not occupied.
  Vec2i getRandomSpawn() {
    std::uniform_int_distribution<int> dist(0, data.size() - 1);

    while (true) {
      int position = dist(rng);
      if (!data[position]->isOccupied()) {
        // Convert position into coordinates.
        int x = position % _width;
        int y = position / _width;
        return {x, y};
      }
    }
  }

  std::queue<Vec2i> pathfind(Vec2i src, Vec2i dest) {
    // Check bounds and ensure movement is possible.
    if (!inBounds(src.x, src.y) || !inBounds(dest.x, dest.y) ||
        at(src.x, src.y)->isOccupied() || at(dest.x, dest.y)->isOccupied()) {
      return std::queue<Vec2i>();
    }

    // Wrap the map data for processing.
    auto grid = std::make_unique<pathfind::LinearGrid<std::shared_ptr<Tile>>>(
        data, _width);

    pathfind::AStar<std::shared_ptr<Tile>> astar(std::move(grid), false);

    // Find the path using the A* algorithm.
    std::vector<Vec2i> path_vec = astar.findPath(src, dest);
    std::queue<Vec2i> path_queue;
    for (auto &step : path_vec) {
      path_queue.push(step);
    }

    return path_queue;
  }

private:
  // Applies a collapsed wave and expands its results into a larger map.
  void applyWave(wfc::Wave<int> wave) {
    int height = wave.size();
    int width = wave[0].size();
    int new_size = TileExpander::DIMENSIONS;

    // Adjust the map to fit the tile data.
    _height = height * new_size;
    _width = width * new_size;
    data.resize(_height * _width);

    // Fill the expanded map with tiles from tilesets.
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        std::vector<std::vector<int>> block =
            TileExpander::expand(rng, wave[i][j].state());
        for (int di = 0; di < new_size; di++) {
          for (int dj = 0; dj < new_size; dj++) {
            data[(new_size * i + di) * _width + (new_size * j + dj)] =
                Tile::build(block[di][dj]);
          }
        }
      }
    }
  }
};

#endif