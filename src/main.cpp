#include "core/core.hpp"
#include <random>

int main(int argc, char const *argv[]) {
  std::mt19937 rng(std::random_device{}());
  core::GameObject game(rng, 128, 128);
  game.registerSystem(core::pathfinder);

  game.start();
  return 0;
}
