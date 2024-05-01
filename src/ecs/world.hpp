#ifndef _ECS_WORLD_HPP
#define _ECS_WORLD_HPP

#include "../map/map.hpp"
#include "sparse.hpp"
#include <cassert>
#include <functional>
#include <tuple>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace ecs {

class World {
private:
  Entity next_id = INVALID_ENTITY + 1; // Next entity id.
  std::vector<Entity> entities;        // All entities tracked.
  std::vector<std::function<void(World &, MapData &)>> systems;

  // Component storage.
  template <typename T> SparseSet<T> &getComponentSet() {
    static std::unordered_map<std::type_index, void *> components;
    auto type = std::type_index(typeid(T));
    if (components.find(type) == components.end()) {
      components[type] = new SparseSet<T>();
    }

    return *static_cast<SparseSet<T> *>(components[type]);
  }

public:
  // Create a new entity.
  Entity createEntity() {
    Entity entity = next_id++;
    entities.push_back(entity);
    return entity;
  }

  // Add an entity and component.
  template <typename T, typename... Args>
  void addComponent(Entity entity, Args &&...args) {
    getComponentSet<T>().add(entity, T(std::forward<Args>(args)...));
  }

  // Removes a component from an entity.
  template <typename T> void removeComponent(Entity entity) {
    getComponentSet<T>().remove(entity);
  }

  // Obtains a single component belonging to an entity.
  template <typename T> T *getComponent(Entity entity) {
    return getComponentSet<T>().get(entity);
  }

  // Function to query entities that have both A and B components.
  template <typename A> std::vector<std::tuple<Entity, A *>> getComponents() {
    std::vector<std::tuple<Entity, A *>> result;
    for (auto entity : entities) {
      A *a = getComponent<A>(entity);
      if (a) {
        result.emplace_back(entity, a);
      }
    }

    return result;
  }

  // Function to query entities that have both A and B components.
  template <typename A, typename B>
  std::vector<std::tuple<Entity, A *, B *>> getComponents() {
    std::vector<std::tuple<Entity, A *, B *>> result;
    for (auto entity : entities) {
      A *a = getComponent<A>(entity);
      B *b = getComponent<B>(entity);
      if (a && b) {
        result.emplace_back(entity, a, b);
      }
    }

    return result;
  }

  // Adds a new system to the world.
  void addSystem(std::function<void(World &, MapData &)> system) {
    systems.push_back(system);
  }

  // Processes all systems.
  void update(MapData &obj) {
    for (auto &system : systems) {
      system(*this, obj);
    }
  }
};

} // namespace ecs

#endif
