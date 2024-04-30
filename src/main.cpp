#include "examples/examples.hpp"
#include "input.hpp"
#include "map/map.hpp"
#include "pathfinding/astar.hpp"
#include "pathfinding/util.hpp"
#include "ui/camera.hpp"
#include "ui/color.hpp"
#include "util/log.hpp"
#include <format>
#include <queue>
#include <random>

const int FRAMERATE = 20;

int main(int argc, char const *argv[]) {
  Log rhs = Log(100);
  std::vector<LogEntry> bhs;
  int width = 128, height = 128;

  // Creates the map.
  std::mt19937 rng(std::random_device{}());
  MapData map(rng, height, width);
  Vec2i player = map.getRandomSpawn();
  std::queue<Vec2i> path;

  // Camera, controls the relative map positioning and optional text.
  Camera camera;
  camera.draw(map, player, rhs.getText(), bhs);

  TickController tick_controller(1000 / FRAMERATE);

  while (true) {
    bool changes = false;
    Input input = Input::check(false);

    if (path.empty()) {
      // Creates a random location within 50 tiles of the player to move to.
      std::uniform_int_distribution<int> dist(-50, 50);
      Vec2i target = Vec2i(dist(rng), dist(rng)) + player;
      path = map.pathfind(player, target);
    }

    if (input.quit) {
      break;
    } else if (!path.empty()) {
      changes = true;
      player = path.front(); // Update player location to next position.
      path.pop();            // Ensure we remove the location we moved to.
    } else if (!input.movement_offset.isOrigin()) {
      changes = true;

      // Input moved in a direction.
      Vec2i temp = player + input.movement_offset;
      auto tile = map.at(temp.x, temp.y);
      if ((!map.inBounds(temp.x, temp.y) ||
           (tile != nullptr && tile->isOccupied()))) {
        // Location is blocked here.
        rhs.add(color::Foreground::RED, color::Style::BOLD, "location blocked");
      } else {
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
