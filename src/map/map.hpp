#ifndef _MAP_DATA_HPP
#define _MAP_DATA_HPP

#include "../generation/terrain/wfc.hpp"
#include "../pathfinding/util.hpp"
#include "../tileset.hpp"
#include "tile.hpp"
#include <memory>
#include <random>
#include <vector>

class MapData {
public:
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

private:
  std::mt19937 rng;

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