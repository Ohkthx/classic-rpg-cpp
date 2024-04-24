#ifndef _ALGO_EXAMPLES_HPP
#define _ALGO_EXAMPLES_HPP

#include "../tick.hpp"
#include <iostream>

// Demonstrates how to initialize and visualize the WFC algorithm.
void wfcExample(int height, int width);

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