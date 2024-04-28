#include "examples/examples.hpp"
#include "input.hpp"
#include "map/map.hpp"
#include "pathfinding/astar.hpp"
#include "pathfinding/util.hpp"
#include "ui/camera.hpp"
#include "ui/color.hpp"
#include "util/log.hpp"
#include <random>

const int FRAMERATE = 30;

int main(int argc, char const *argv[]) {
  // Test the tick controller @30 ticks per second.
  // tickExample(1000 / FRAMERATE);

  // Test the Wave Function Collapse algorithm.
  // auto wave = wfcExample(25, 25, true, false);

  // Test the Tileset + Wave Function Collapse algorithm.
  // tilesetExample(wave);

  // A* Pathfinding algorithm. 'true' for allow diagonal movement.
  // astarExample(25, 25, false);

  int width = 128, height = 128;
  Vec2i player = {width / 2, height / 2};

  std::mt19937 rng(std::random_device{}());
  MapData map(rng, height, width);
  Log rhs = Log(100);
  std::vector<LogEntry> bhs;

  // Camera, controls the relative map positioning and optional text.
  Camera camera;
  camera.draw(map, player, rhs.getText(), bhs);

  TickController tick_controller(1000 / FRAMERATE);

  while (true) {
    bool changes = false;
    Input input = Input::check(false);

    if (input.quit) {
      break;
    } else if (!input.movement_offset.isOrigin()) {
      // Input moved in a direction.
      Vec2i temp = player + input.movement_offset;
      auto tile = map.at(temp.x, temp.y);
      if ((!map.inBounds(temp.x, temp.y) ||
           (tile != nullptr && tile->isOccupied()))) {
        // Location is blocked here.
        rhs.add(color::Foreground::RED, color::Style::BOLD, "location blocked");
      } else {
        changes = true;
        player = temp;
        if (input.movement_offset.x > 0) {
          rhs.add("moved east");
        } else if (input.movement_offset.x < 0) {
          rhs.add("moved west");
        } else if (input.movement_offset.y > 0) {
          rhs.add("moved south");
        } else if (input.movement_offset.y < 0) {
          rhs.add("moved north");
        }
      }
    }

    if (changes) {
      camera.draw(map, player, rhs.getText(), bhs);
    }

    tick_controller.tick();
  }

  return 0;
}
