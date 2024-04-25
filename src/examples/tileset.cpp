#include "../tileset.hpp"
#include "../generation/terrain/wfc.hpp"
#include <iostream>
#include <random>

wfc::Ruleset<int> getRules() {
  const int G = 20;
  const int C = 100;
  const int CI = C * 10;
  const int CO = CI / 2;
  const int S = 10;
  const int W = 10000;

  wfc::Ruleset<int> rules;
  // Grass
  rules.addRule(0, G, {{0, 19}, {0, 19}, {0, 19}, {0, 19}});

  // Sand
  rules.addRule(19, S,
                {{0, 19, 21, 23, 32},
                 {0, 19, 24, 23, 26},
                 {0, 19, 27, 26, 29},
                 {0, 19, 30, 29, 32}});
  // Water
  rules.addRule(
      20, W,
      {{20, 25, 27, 28}, {20, 28, 30, 31}, {20, 21, 22, 31}, {20, 22, 24, 25}});
  rules.addRule(21, C, {{20}, {21, 22, 32}, {19}, {21, 31, 23}});  // N
  rules.addRule(22, CO, {{20}, {20}, {24, 25, 23}, {21, 31, 23}}); // NE
  rules.addRule(23, CI, {{22, 24, 26}, {21, 22, 32}, {19}, {19}}); // NER
  rules.addRule(24, C, {{22, 24, 26}, {20}, {24, 25, 23}, {19}});  // E
  rules.addRule(25, CO, {{22, 24, 26}, {20}, {20}, {27, 28, 26}}); // SE
  rules.addRule(26, CI, {{19}, {25, 27, 29}, {24, 25, 23}, {19}}); // SER
  rules.addRule(27, C, {{19}, {25, 27, 29}, {20}, {27, 28, 26}});  // S
  rules.addRule(28, CO, {{30, 31, 29}, {25, 27, 29}, {20}, {20}}); // SW
  rules.addRule(29, CI, {{19}, {19}, {28, 30, 32}, {27, 28, 26}}); // SWR
  rules.addRule(30, C, {{30, 31, 29}, {19}, {28, 30, 32}, {20}});  // W
  rules.addRule(31, CO, {{20}, {21, 22, 32}, {28, 30, 32}, {20}}); // NW
  rules.addRule(32, CI, {{30, 31, 29}, {19}, {19}, {21, 31, 23}}); // NWR

  rules.validate();
  return rules;
}

std::string resetSymbol() { return "\033[0m"; }

std::string getSymbol(int tile_id) {
  switch (tile_id) {
  case 0:
    return "\033[32m▒";
  case 19:
    return "\033[1;37m░";
  case 20:
    return "\033[34m≈";
  default:
    return "?";
  }
}

void tilesetExample(wfc::Wave<int> &wave) {
  std::mt19937 rng(std::random_device{}());

  // Print tiles in a grid format.
  for (int y = 0; y < wave.size(); ++y) {
    // Each tile is 3x3, hence 3 rows per layer.
    for (int layer = 0; layer < TileExpander::tile_dimensions; ++layer) {
      for (int x = 0; x < wave[y].size(); ++x) {
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