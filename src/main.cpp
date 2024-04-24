#include "examples/examples.hpp"

const int FRAMERATE = 60;

int main(int argc, char const *argv[]) {
  // Test the tick controller @60 ticks per second.
  tickExample(1000 / 60);

  // Test the Wave Function Collapse algorithm.
  // wfcExample(25, 25);

  return 0;
}
