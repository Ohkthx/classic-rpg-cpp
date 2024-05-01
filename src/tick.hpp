#ifndef _TICK_CONTROLLER_HPP
#define _TICK_CONTROLLER_HPP

#include <chrono>
#include <format>
#include <thread>

class TickController {
public:
  // Initializes the tick controller with the specified interval in
  // milliseconds.
  TickController(std::size_t tick_interval)
      : tick_interval(tick_interval),
        last_tick(std::chrono::steady_clock::now()) {}

  // Returns the number of ticks that have occurred.
  std::size_t getTick() const { return tick_count; }

  // Resets the internal tick data.
  void restart() { start(); }

  // Resets the internal tick data.
  void start() {
    last_tick = std::chrono::steady_clock::now();
    tick_count = 0;
  }

  // Proceeds the controller to the next tick, ensuring it happens at the
  // correct interval.
  void tick() {
    auto now = std::chrono::steady_clock::now();
    auto next_tick_time = last_tick + std::chrono::milliseconds(tick_interval);
    if (now < next_tick_time) {
      std::this_thread::sleep_until(next_tick_time);
    }

    last_tick = next_tick_time;
    tick_count++;
  }

private:
  std::size_t tick_count = 1; // Tick counter.
  std::size_t tick_interval;  // Amount of time in ms between ticks.
  std::chrono::steady_clock::time_point last_tick; // Last tick since epoch.
};

#endif