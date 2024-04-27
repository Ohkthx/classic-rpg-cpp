#include "examples/examples.hpp"
#include "map/map.hpp"
#include "ui/terminal.hpp"
#include <random>

const int FRAMERATE = 60;

int main(int argc, char const *argv[]) {
  // Test the tick controller @60 ticks per second.
  // tickExample(1000 / 60);

  // Test the Wave Function Collapse algorithm.
  // auto wave = wfcExample(25, 25, true, false);

  // Test the Tileset + Wave Function Collapse algorithm.
  // tilesetExample(wave);

  // A* Pathfinding algorithm. 'true' for allow diagonal movement.
  // astarExample(25, 25, false);

  std::mt19937 rng(std::random_device{}());
  MapData map(rng, 20, 40);

  Terminal terminal;
  terminal.draw(map);

  return 0;
}
