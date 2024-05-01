#include "gameobj.hpp"
#include "../input.hpp"
#include "components.hpp"

GameObject::GameObject(std::mt19937 &rng, int width, int height)
    : rng(rng), map(MapData(rng, height, width)),
      ticks(TickController(1000 / FRAMERATE)), rhs(Log(100)) {
  // Creates the map and place the player.
  player = world.createEntity();
  last_position = map.getRandomSpawn();
  PositionComponent pos = PositionComponent(last_position);
  world.addComponent<PositionComponent>(player, pos);

  std::queue<Vec2i> path;
  world.addComponent<PathComponent>(player, path);
}

void GameObject::registerSystem(
    std::function<void(ecs::World &, MapData &)> system) {
  world.addSystem(system);
}

void GameObject::start() {
  PositionComponent *pos = world.getComponent<PositionComponent>(player);
  view.draw(map, *pos, rhs.getText(), bhs);
  ticks.start();

  while (true) {
    // Process all of the systems.
    world.update(this->map);
    pos = world.getComponent<PositionComponent>(player);

    // Get user input.
    Input input = Input::check(false);

    if (input.quit) {
      break;
    } else if (!input.movement_offset.isOrigin()) {
      // Input moved in a direction.
      Vec2i temp = *pos + input.movement_offset;
      auto tile = map.at(temp.x, temp.y);
      if ((!map.inBounds(temp.x, temp.y) ||
           (tile != nullptr && tile->isOccupied()))) {
        // Location is blocked here.
        rhs.add(color::Foreground::RED, color::Style::BOLD, "location blocked");
      } else {
        *pos = temp;
      }
    }

    // Update the RHS log.
    if (last_position != *pos) {
      if (pos->x > last_position.x) {
        rhs.add("moved east");
      } else if (pos->x < last_position.y) {
        rhs.add("moved west");
      } else if (pos->y > last_position.y) {
        rhs.add("moved south");
      } else if (pos->y < last_position.y) {
        rhs.add("moved north");
      }
      last_position = *pos;
    }

    view.draw(map, *pos, rhs.getText(), bhs);
    ticks.tick();
  }
}
