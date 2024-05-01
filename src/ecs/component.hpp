#ifndef _ECS_COMPONENT_HPP
#define _ECS_COMPONENT_HPP

#include "../pathfinding/util.hpp"
#include <queue>

namespace ecs {

// Base Component class for type identification.
struct Component {
  virtual ~Component() = default;
};

} // namespace ecs

#endif