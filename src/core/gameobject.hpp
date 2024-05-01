#ifndef _CORE_GAME_OBJECT_HPP
#define _CORE_GAME_OBJECT_HPP

#include "../ecs/ecs.hpp"
#include "../map/map.hpp"
#include "../pathfind/pathfind.hpp"
#include "../tick.hpp"
#include "../ui/camera.hpp"
#include "../util/log.hpp"
#include <random>

namespace core {

class GameObject {
private:
  const int FRAMERATE = 20; // Amount of frames per second to render.
  Camera view;              // Camera / Terminal renderer.
  ecs::Entity player;       // Player entity ID.
  Vec2i last_position = Vec2i::ORIGIN(); // Last position for player.
  TickController ticks;                  // Tick speed and pause.
  Log rhs;                               // Right-hand side Log.
  std::vector<LogEntry> bhs;             // Bottom log.
  std::mt19937 rng;

public:
  ecs::World world; // ECS / World controller.
  MapData map;      // Map data.

  GameObject(std::mt19937 &rng, int width, int height);

  // Registers a new system within the ECS.
  void registerSystem(std::function<void(ecs::World &, MapData &)> system);
  void start(); // Starts the game loop.
};

} // namespace core

#endif