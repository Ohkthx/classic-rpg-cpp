#ifndef _MAP_DATA_HPP
#define _MAP_DATA_HPP

#include "../generation/terrain/wfc.hpp"
#include "../tileset.hpp"
#include "tile.hpp"
#include <memory>
#include <random>
#include <vector>

class MapData {
public:
  std::vector<std::vector<std::shared_ptr<Tile>>> data;

  MapData(std::mt19937 &rng, int height, int width,
          wfc::Ruleset<int> rules = wfc::Ruleset<int>::DefaultRules())
      : rng(rng) {
    // Initialize and collapse the map using WFC.
    wfc::WaveFunctionCollapse wfc(rng, height, width, rules, false);
    wfc.collapse();

    // Apply the wave to the map.
    applyWave(wfc.getWave());
  }

private:
  std::mt19937 rng;

  // Applies a collapsed wave and expands its results into a larger map.
  void applyWave(wfc::Wave<int> wave) {
    int height = wave.size();
    int width = wave[0].size();
    int new_size = TileExpander::DIMENSIONS;

    // Adjust the map to fit the tile data.
    data.resize(height * new_size,
                std::vector<std::shared_ptr<Tile>>(width * new_size));

    // Fill the expanded map with the tile from tilesets.
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        std::vector<std::vector<int>> block =
            TileExpander::expand(rng, wave[i][j].state());
        for (int di = 0; di < new_size; di++) {
          for (int dj = 0; dj < new_size; dj++) {
            data[new_size * i + di][new_size * j + dj] =
                Tile::build(block[di][dj]);
          }
        }
      }
    }
  }
};

#endif