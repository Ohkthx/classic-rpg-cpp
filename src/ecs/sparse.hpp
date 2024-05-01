#ifndef _ECS_SPARSE_SET_HPP
#define _ECS_SPARSE_SET_HPP

#include "component.hpp"
#include <cassert>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace ecs {

// An ID to represent an entity.
using Entity = std::size_t;
const Entity INVALID_ENTITY = -1;

// Sparse set for efficient component storage.
template <typename T> class SparseSet {
private:
  std::vector<std::size_t> sparse; // Maps entity IDs to dense indices.
  std::vector<Entity> dense;       // Stores entity IDs.
  std::vector<T> components;       // Stores components.

public:
  SparseSet() {
    static_assert(std::is_base_of<Component, T>::value,
                  "T must be a Component");
  }

  // Add a new component to an entity.
  void add(Entity entity, const T &component) {
    if (entity >= sparse.size()) {
      sparse.resize(entity + 1, INVALID_ENTITY);
    }

    // Add the component if it does not exist already.
    if (sparse[entity] == INVALID_ENTITY) {
      sparse[entity] = dense.size();
      dense.push_back(entity);
      components.push_back(component);
    } else {
      components[sparse[entity]] = component;
    }
  }

  // Removes an entity and its components from the Sparse Set.
  void remove(Entity entity) {
    if (entity >= sparse.size() || sparse[entity] == INVALID_ENTITY) {
      return;
    }

    Entity lastEntity = dense.back();
    std::size_t indexToRemove = sparse[entity];
    dense[indexToRemove] = lastEntity;
    dense.pop_back();

    components[indexToRemove] = components.back();
    components.pop_back();

    sparse[lastEntity] = indexToRemove;
    sparse[entity] = INVALID_ENTITY;
  }

  // Obtains a component belonging to an entity.
  T *get(Entity entity) {
    if (entity < sparse.size() && sparse[entity] != INVALID_ENTITY) {
      return &components[sparse[entity]];
    }

    return nullptr;
  }
};

} // namespace ecs

#endif