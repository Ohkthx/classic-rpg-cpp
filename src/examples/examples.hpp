#ifndef _ALGO_EXAMPLES_HPP
#define _ALGO_EXAMPLES_HPP

#include "../generation/terrain/wfc.hpp"
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

#endif