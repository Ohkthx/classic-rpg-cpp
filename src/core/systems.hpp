#ifndef _CORE_SYSTEMS_HPP
#define _CORE_SYSTEMS_HPP

#include "../ecs/ecs.hpp"
#include "../map/map.hpp"
#include "components.hpp"

namespace core {

// Selects a random path for all entities that are currently pathfinding.
void pathfinder(ecs::World &world, MapData &map) {
  auto entities = world.getComponents<PositionComponent, PathComponent>();
  for (auto &entity : entities) {
    auto [e, pos, pathing] = entity;
    if (auto next = pathing->next(); next) {
      // There is a next position to move to.
      *pos = *next;
    } else {
      // Generate a new position to move to.
      static std::uniform_int_distribution<int> dist(-50, 50);
      Vec2i target = Vec2i(dist(map.rng), dist(map.rng)) + *pos;
      pathing->path = map.pathfind(*pos, target);
    }
  }
}

} // namespace core

#endif