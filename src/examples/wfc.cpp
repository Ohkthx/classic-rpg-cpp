#include "../generation/terrain/wfc.hpp"
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <thread>
#include <vector>

std::string getSymbol(int value) {
  switch (value) {
  case 0:
    return "\033[32m░";
  case 19:
    return "\033[1;37m▒";
  case 20:
    return "\033[34m≈";
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
    return "\033[32m" + std::to_string(entropy) + " \033[0m";
  } else if (entropy < 5) {
    // Yellow for moderate entropy
    return "\033[33m" + std::to_string(entropy) + " \033[0m";
  } else if (entropy < 10) {
    // Red for high entropy
    return "\033[31m" + std::to_string(entropy) + " \033[0m";
  } else {
    // Magenta for very high entropy
    return "\033[35m" + std::to_string(entropy) + "\033[0m";
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
        output << getSymbol(cell.state()) << "";
      }

      // Reset the terminal styles.
      output << "\033[0m";
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

void wfcExample(int height, int width) {
  const int G = 20;
  const int C = 100;
  const int CI = C * 10;
  const int CO = CI / 2;
  const int S = 10;
  const int W = 10000;

  wfc::Ruleset<int> rules;
  // Grass
  rules.addRule(0, G, {{0, 19}, {0, 19}, {0, 19}, {0, 19}});

  // Sand - Outer
  rules.addRule(19, S,
                {{0, 19, 21, 23, 32},
                 {0, 19, 24, 23, 26},
                 {0, 19, 27, 26, 29},
                 {0, 19, 30, 29, 32}});
  // Water - Inner
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

  std::mt19937 rng(std::random_device{}());
  int delay = 100;
  bool debug = false, clear = true;

  wfc::WaveFunctionCollapse<int> wfc(rng, height, width, rules, true);
  auto wave = wfc.getWave();

  // Collapse the entire wave.
  while (!wfc.isCollapsed()) {
    wfc.next();
  }

  // Update the wave animation.
  wave = wfc.getWave();
  animateScroll(wave, debug, clear, 3000000, delay);
}