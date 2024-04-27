#include "../tileset.hpp"
#include "../generation/terrain/wfc.hpp"
#include "../ui/color.hpp"
#include <iostream>
#include <random>

std::string resetSymbol() { return "\033[0m"; }

std::string getSymbol(int tile_id) {
  switch (tile_id) {
  case 0:
    return color::stylize("░", color::Foreground::GREEN);
  case 19:
    return color::stylize("▒", color::Foreground::BRIGHT_YELLOW);
  case 20:
    return color::stylize("≈", color::Foreground::BLUE);
  default:
    return "?";
  }
}

void tilesetExample(wfc::Wave<int> &wave) {
  std::mt19937 rng(std::random_device{}());

  // Print tiles in a grid format.
  for (int y = 0; y < wave.size(); y++) {
    for (int layer = 0; layer < TileExpander::DIMENSIONS; layer++) {
      for (int x = 0; x < wave[y].size(); x++) {
        int state = wave[y][x].state();
        std::vector<std::vector<int>> tile = TileExpander::expand(rng, state);

        for (const int &cell : tile[layer]) {
          std::cout << getSymbol(cell) << resetSymbol() << "";
        }
      }

      std::cout << std::endl;
    }
  }
}