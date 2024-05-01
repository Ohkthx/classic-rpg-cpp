#ifndef _PATHFIND_ASTAR_HPP
#define _PATHFIND_ASTAR_HPP

#include "grid.hpp"
#include "util.hpp"
#include <algorithm>
#include <functional>
#include <memory>
#include <queue>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace pathfind {

// Heuristics used to calculate cost / distance differently.
enum class HeuristicType { Manhattan, Chebyshev, Euclidean };

class HeuristicFunction {
public:
  static int Manhattan(Vec2i start, Vec2i end) {
    return std::abs(start.x - end.x) + std::abs(start.y - end.y);
  }

  static int Chebyshev(Vec2i start, Vec2i end) {
    return std::max(std::abs(start.x - start.y), std::abs(end.y - start.y));
  }

  static int Euclidean(Vec2i start, Vec2i end) {
    return std::sqrt((end.x - start.x) * (end.x - start.x) +
                     (end.y - start.y) * (end.y - start.y));
  }

  // Function that selects the appropriate heuristic based on type.
  static std::function<int(Vec2i, Vec2i)> getHeuristic(HeuristicType type) {
    switch (type) {
    case HeuristicType::Chebyshev:
      return Chebyshev;
    case HeuristicType::Euclidean:
      return Euclidean;
    default:
      return Manhattan;
    }
  }
};

struct Node {
  Vec2i position;
  Node *parent;
  int g, h; // Cost to start node, heuristic to end node.

  Node(Vec2i pos, Node *p = nullptr, int g_cost = 0, int h_cost = 0)
      : position(pos), parent(p), g(g_cost), h(h_cost) {}

  int f() const { return g + h; } // Total cost.
};

struct CompareNode {
  bool operator()(Node *a, Node *b) const { return a->f() > b->f(); }
};

template <Occupiable T> class AStar {
public:
  AStar(std::unique_ptr<Grid<T>> grid, bool allow_diagonal = false,
        bool wrap = false, HeuristicType ht = HeuristicType::Manhattan)
      : grid(std::move(grid)), wrap(wrap),
        heuristic(HeuristicFunction::getHeuristic(ht)) {
    Vec2i dimensions = this->grid->getDimensions();
    if (dimensions.x == 0 || dimensions.y == 0) {
      throw std::runtime_error("Invalid dimensions for grid provided.");
    }

    // Set the diagonal ability.
    offsets = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
    if (allow_diagonal) {
      std::vector<Vec2i> ext = {{1, -1}, {1, 1}, {-1, 1}, {-1, -1}};
      offsets.insert(offsets.end(), ext.begin(), ext.end());
    }
  }

  int getHeight() const { return height; } // Height of the grid.
  int getWidth() const { return width; }   // Width of the grid.

  // Finds the path from start to end.
  std::vector<Vec2i> findPath(const Vec2i &start, const Vec2i &end) {
    if (!grid->isValid(start) || !grid->isValid(end)) {
      throw new std::runtime_error(
          "Invalid start or end position for pathfinding.");
    }

    std::priority_queue<Node *, std::vector<Node *>, CompareNode> open_list;
    std::unordered_map<Vec2i, std::unique_ptr<Node>> all_nodes;

    all_nodes[start] =
        std::make_unique<Node>(start, nullptr, 0, heuristic(start, end));
    open_list.push(all_nodes[start].get());

    while (!open_list.empty()) {
      Node *current = open_list.top();
      open_list.pop();

      // Algorithm is complete, it found a path.
      if (current->position == end) {
        return reconstructPath(current);
      }

      // Explore the neighbors of the current node.
      for (Vec2i neighbor_pos : getNeighbors(current->position)) {
        if (is_occupiable_impl<T>::check(
                grid->at(neighbor_pos.x, neighbor_pos.y))) {
          continue;
        }

        // Check if the neighbor has not been visited or a cheaper path is
        // found.
        int g_score = current->g + 1;
        if (!all_nodes.count(neighbor_pos) ||
            g_score < all_nodes[neighbor_pos]->g) {
          all_nodes[neighbor_pos] = std::make_unique<Node>(
              neighbor_pos, current, g_score, heuristic(neighbor_pos, end));
          open_list.push(all_nodes[neighbor_pos].get());
        }
      }
    }

    return {};
  }

private:
  int height, width;             // Dimensions of the inner grid.
  bool wrap;                     // Allow looping around borders of the grid.
  std::unique_ptr<Grid<T>> grid; // Grid being processed.
  std::vector<Vec2i> offsets;    // Offsets to get neighboring cells.
  std::function<int(Vec2i, Vec2i)> heuristic; // Heuristic for distance / cost.

  // Obtains all neighbors to the position while respecting grid boundaries.
  std::vector<Vec2i> getNeighbors(const Vec2i &position) {
    std::vector<Vec2i> neighbors;
    for (const Vec2i &offset : offsets) {
      Vec2i neighbor = position + offset;

      if (wrap) {
        neighbor.x = (neighbor.x + width) % width;
        neighbor.y = (neighbor.y + height) % height;
        neighbors.push_back(neighbor);
      } else if (grid->isValid(neighbor)) {
        neighbors.push_back(neighbor);
      }
    }

    return neighbors;
  }

  // Reconstructs the path starting at the current node to the start.
  std::vector<Vec2i> reconstructPath(Node *current_node) {
    std::vector<Vec2i> path;

    while (current_node) {
      path.push_back(current_node->position);
      current_node = current_node->parent;
    }

    std::reverse(path.begin(), path.end());
    return path;
  }
};
} // namespace pathfind

#endif