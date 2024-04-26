#include "../generation/terrain/wfc.hpp"
#include "../ui/color.hpp"
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <thread>
#include <vector>

std::string getWFCSymbol(int value) {
  switch (value) {
  case 0:
    return color::stylize("░", color::Foreground::GREEN);
  case 19:
    return color::stylize("▒", color::Foreground::YELLOW);
  case 20:
    return color::stylize("≈", color::Foreground::BLUE);
  case 21:
  case 27:
    return "━";
  case 22:
  case 29:
    return "┓";
  case 23:
  case 28:
    return "┗";
  case 24:
  case 30:
    return "┃";
  case 25:
  case 32:
    return "┛";
  case 31:
  case 26:
    return "┏";

  default:
    return "?";
  }
}

std::string getColorEntropy(int entropy) {
  if (entropy < 3) {
    // Green for low entropy
    return color::stylize(std::to_string(entropy), color::Foreground::GREEN);
  } else if (entropy < 5) {
    // Yellow for moderate entropy
    return color::stylize(std::to_string(entropy), color::Foreground::YELLOW);
  } else if (entropy < 10) {
    // Red for high entropy
    return color::stylize(std::to_string(entropy), color::Foreground::RED);
  } else {
    // Magenta for very high entropy
    return color::stylize(std::to_string(entropy), color::Foreground::MAGENTA);
  }
}

void print(std::vector<std::vector<wfc::Cell<int>>> &wave, bool debug,
           bool clear, int delay, int offsetx = 0, int offsety = 0) {
  std::ostringstream output;
  if (clear) {
    // Clear screen and move cursor to home position
    output << "\033[2J\033[1;1H";
  }

  int height = wave.size();
  int width = wave[0].size();

  for (int y = 0; y < height; ++y) {
    int rowIdx = (y + offsety) % height;
    if (rowIdx < 0)
      rowIdx += height; // Correct negative index.

    for (int x = 0; x < width; ++x) {
      int colIdx = (x + offsetx) % width;
      if (colIdx < 0)
        colIdx += width; // Correct negative index.

      wfc::Cell<int> &cell = wave[rowIdx][colIdx];

      if (debug) {
        output << std::setw(2) << std::left << getColorEntropy(cell.entropy());
      } else if (!cell.isCollapsed()) {
        output << std::setw(2) << std::left << getColorEntropy(cell.entropy());
      } else {
        output << getWFCSymbol(cell.state()) << "";
      }
    }
    output << std::endl;
  }

  std::cout << output.str();
  std::cout.flush();
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
}
void animateScroll(std::vector<std::vector<wfc::Cell<int>>> &wave, bool debug,
                   bool clear, int totalDuration, int frameDelay) {
  std::mt19937 rng(std::random_device{}());

  // Distribution for changing directions.
  std::uniform_int_distribution<int> dist(-1, 1);

  int offsetx = 0;
  int offsety = 0;
  int directionX = 1;
  int directionY = 0;

  auto startTime = std::chrono::steady_clock::now();
  auto endTime = startTime + std::chrono::milliseconds(totalDuration);

  while (std::chrono::steady_clock::now() < endTime) {
    // Update the offsets.
    offsetx += directionX;
    offsety += directionY;

    // Print the current state of the wave grid with current offsets.
    print(wave, debug, clear, frameDelay, offsetx, offsety);

    // Randomly change directions every few frames.
    if (std::chrono::steady_clock::now() - startTime >
        std::chrono::milliseconds(1000)) {
      directionX = dist(rng);
      directionY = dist(rng);

      // Reset change direction timer.
      startTime = std::chrono::steady_clock::now();
    }

    // Delay between frames.
    std::this_thread::sleep_for(std::chrono::milliseconds(frameDelay));
  }
}

wfc::Wave<int> wfcExample(int height, int width, bool display, bool animate) {
  std::mt19937 rng(std::random_device{}());
  wfc::Ruleset<int> rules = wfc::Ruleset<int>::DefaultRules();
  int delay = 100;
  bool debug = false, clear = true;

  wfc::WaveFunctionCollapse<int> wfc(rng, height, width, rules, true);

  // Collapse the entire wave.
  while (!wfc.isCollapsed()) {
    wfc.next();
  }

  // Update the wave animation.
  auto wave = wfc.getWave();
  if (display && animate) {
    animateScroll(wave, debug, clear, 30000, delay);
  } else if (display) {
    print(wave, false, false, 0);
  }

  return wave;
}