#include "gameobj/gameobj.hpp"
#include "gameobj/systems.hpp"
#include <random>

int main(int argc, char const *argv[]) {
  std::mt19937 rng(std::random_device{}());
  GameObject game(rng, 128, 128);
  game.registerSystem(pathfinder);

  game.start();
  return 0;
}
