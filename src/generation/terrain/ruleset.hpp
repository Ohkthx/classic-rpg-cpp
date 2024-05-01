#ifndef _WAVE_FUNCTION_COLLAPSE_RULESET_HPP
#define _WAVE_FUNCTION_COLLAPSE_RULESET_HPP

#include <algorithm>
#include <map>
#include <random>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace wfc {

template <typename T> class Rule {
public:
  Rule(int weight, const std::vector<std::vector<T>> &data)
      : weight_(weight), data(data) {}

  // Returns the weight of the rule.
  int weight() const { return weight_; }

  // Returns the possible options at the given direction.
  // Direction should be an index within the range of data vector.
  const std::vector<T> &atDirection(int direction) const {
    return data.at(direction);
  }

private:
  int weight_;
  std::vector<std::vector<T>> data;
};

template <typename T> class Ruleset {
public:
  Ruleset() {}

  // Retrieves a rule by ID. Throws std::out_of_range if the ID is not found.
  const Rule<T> &getRule(T id) const { return rules.at(id); }

  // Adds a new rule to the ruleset.
  void addRule(T id, int weight, std::vector<std::vector<T>> data) {
    for (auto &vec : data) {
      std::sort(vec.begin(), vec.end());
    }

    rules.emplace(std::piecewise_construct, std::forward_as_tuple(id),
                  std::forward_as_tuple(weight, data));
  }

  // Obtains all possible rules.
  std::vector<T> allRules() const {
    std::vector<T> all_rules;

    all_rules.reserve(rules.size());
    for (const auto &pair : rules) {
      all_rules.push_back(pair.first);
    }

    std::sort(all_rules.begin(), all_rules.end());
    return all_rules;
  }

  // Selects a rule from the rule ids provided, determined by the weights.
  T pickRule(std::mt19937 &rng, const std::vector<T> &rule_ids) {
    std::vector<int> weights;
    for (int id : rule_ids) {
      weights.push_back(rules.at(id).weight());
    }

    std::discrete_distribution<int> dist(weights.begin(), weights.end());
    return rule_ids[dist(rng)];
  }

  // Validates that each of the rules has reverse access to each other.
  void validate() {
    for (const auto &[key, rule] : rules) {
      for (int dir = 0; dir < 4;
           dir++) { // Assuming there are always 4 directions
        const auto &possible_rules = rule.atDirection(dir);
        for (int linked : possible_rules) {
          int reverse = oppositeDirection(dir);
          const auto &reverse_rules = rules.at(linked).atDirection(reverse);
          if (std::find(reverse_rules.begin(), reverse_rules.end(), key) ==
              reverse_rules.end()) {
            std::ostringstream ss;
            ss << "Validation Error: Missing reverse rule from " << linked
               << " to " << key << " in direction " << reverse;
            throw std::runtime_error(ss.str());
          }
        }
      }
    }
  }

  // Default ruleset, used for testing and cleanliness of code.
  static Ruleset<int> DefaultRules() {
    // Weights
    const int G = 20;      // Grass
    const int C = 100;     // Coast
    const int CI = C * 10; // Inner Coast
    const int CO = CI / 2; // Outer Coast
    const int S = 10;      // Sand
    const int W = 10000;   // Water

    wfc::Ruleset<int> rules;
    // Grass
    rules.addRule(0, G, {{0, 19}, {0, 19}, {0, 19}, {0, 19}});

    // Sand
    rules.addRule(19, S,
                  {{0, 19, 21, 23, 32},
                   {0, 19, 24, 23, 26},
                   {0, 19, 27, 26, 29},
                   {0, 19, 30, 29, 32}});
    // Water
    rules.addRule(20, W,
                  {{20, 25, 27, 28},
                   {20, 28, 30, 31},
                   {20, 21, 22, 31},
                   {20, 22, 24, 25}});

    // Coastal rules.
    rules.addRule(21, C, {{20}, {21, 22, 32}, {19}, {21, 31, 23}});  // N
    rules.addRule(22, CO, {{20}, {20}, {24, 25, 23}, {21, 31, 23}}); // NE
    rules.addRule(23, CI, {{22, 24, 26}, {21, 22, 32}, {19}, {19}}); // NER
    rules.addRule(24, C, {{22, 24, 26}, {20}, {24, 25, 23}, {19}});  // E
    rules.addRule(25, CO, {{22, 24, 26}, {20}, {20}, {27, 28, 26}}); // SE
    rules.addRule(26, CI, {{19}, {25, 27, 29}, {24, 25, 23}, {19}}); // SER
    rules.addRule(27, C, {{19}, {25, 27, 29}, {20}, {27, 28, 26}});  // S
    rules.addRule(28, CO, {{30, 31, 29}, {25, 27, 29}, {20}, {20}}); // SW
    rules.addRule(29, CI, {{19}, {19}, {28, 30, 32}, {27, 28, 26}}); // SWR
    rules.addRule(30, C, {{30, 31, 29}, {19}, {28, 30, 32}, {20}});  // W
    rules.addRule(31, CO, {{20}, {21, 22, 32}, {28, 30, 32}, {20}}); // NW
    rules.addRule(32, CI, {{30, 31, 29}, {19}, {19}, {21, 31, 23}}); // NWR

    rules.validate();
    return rules;
  }

private:
  std::map<T, Rule<T>> rules;

  // Gets the opposite direction. 0 <-> 2, 1 <-> 3
  int oppositeDirection(int direction) { return (direction + 2) % 4; }
};

} // namespace wfc

#endif