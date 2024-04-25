#ifndef _TILESET_HPP
#define _TILESET_HPP

#include <random>
#include <string>
#include <unordered_map>
#include <vector>

class TileExpander {
public:
  static const int tile_dimensions = 3; // nxn dimensions of each expanded tile.

  // Converts a single tile into an nxn grid of tiles, randomly selecting a
  // configuration if multiple exist.
  static std::vector<std::vector<int>> expand(std::mt19937 &rng, int tile_id) {
    std::vector<TileExpansion> tiles;
    int rotation = 0;

    if (tile_id >= 21 && tile_id <= 32) {
      // Coast regions that can be rotated.
      const auto &[id, rotate] = getTileData(tile_id, 21, 3);
      rotation = rotate;
      tiles = expansionMap(id);
    } else {
      tiles = expansionMap(tile_id);
    }

    // Pick a random tile and rotate it.
    std::uniform_int_distribution<int> dist(0, tiles.size() - 1);
    TileExpansion tile = tiles[dist(rng)];
    rotate(tile, rotation);

    return tile;
  }

private:
  using TileExpansion = std::vector<std::vector<int>>;

  // Expands a single tile by id into an nxn matrix.
  static std::vector<TileExpansion> expansionMap(int tile_id) {
    switch (tile_id) {
    case 0: // Grass
      return {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}};
    case 19: // Sand
      return {{{19, 19, 19}, {19, 19, 19}, {19, 19, 19}}};
    case 20: // Water
      return {{{20, 20, 20}, {20, 20, 20}, {20, 20, 20}}};
    case 21: // Northern Coast
      return {
          {{20, 20, 20}, {19, 19, 19}, {19, 19, 19}},
          {{20, 20, 20}, {19, 19, 19}, {19, 19, 19}},
          {{20, 20, 20}, {19, 19, 19}, {19, 19, 19}},
      };
    case 22: // Northwestern Coast
      return {
          {{20, 20, 20}, {19, 19, 20}, {19, 19, 20}},
          {{20, 20, 20}, {19, 19, 20}, {19, 19, 20}},
          {{20, 20, 20}, {19, 19, 20}, {19, 19, 20}},
      };
    case 23: // Northwestern Coast (inland)
      return {
          {{19, 19, 20}, {19, 19, 19}, {19, 19, 19}},
          {{19, 19, 20}, {19, 19, 19}, {19, 19, 19}},
          {{19, 19, 20}, {19, 19, 19}, {19, 19, 19}},
      };
    default:
      return {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}};
    }
  }

  // Rotates a tile expansion (which must be nxn) in 3 additional
  // directions.
  static void rotate(TileExpansion &tile, int rotations) {
    int n = tile.size();
    if (n != tile_dimensions || tile[0].size() != tile_dimensions) {
      throw new std::runtime_error(
          "Incorrect tileset definition, size mismatch.");
    } else if (rotations % 4 == 0) {
      return;
    }

    // Rotates the matrix n times, copying into new_tile.
    TileExpansion new_tile(n, std::vector<int>(n));
    for (int r = 0; r < rotations % 4; ++r) {
      for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
          new_tile[j][n - 1 - i] = tile[i][j];
        }
      }
      tile = new_tile;
    }
  }

  // Extracts the mapped value of the tile and rotation from the ID.
  static std::pair<int, int> getTileData(int tile_id, int minimum, int types) {
    if (types == 0) {
      throw new std::runtime_error("Tile types cannot be 0.");
    }

    int mapped_id = (tile_id % types) + minimum;
    int rotation = (tile_id % mapped_id) / types;
    return {mapped_id, rotation};
  }
};

#endif