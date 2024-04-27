#ifndef _MAP_TILE_HPP
#define _MAP_TILE_HPP

#include "../ui/color.hpp"
#include <iostream>
#include <random>
#include <string>

enum class TileType { Grass = 0, Sand = 19, Water = 20 };

class Tile {
protected:
  static inline std::mt19937 rng = std::mt19937(std::random_device{}());

public:
  Tile() {}
  virtual ~Tile() {}

  virtual std::string toString() = 0;      // String representation of the tile.
  virtual bool isOccupied() const = 0;     // Checks if the tile can be passed.
  void draw() { std::cout << toString(); } // Draws the tile to screen.
  static std::shared_ptr<Tile> build(int tile_id); // Tile factory.
};

class Grass : public Tile {
private:
  static const std::string symbol;
  const color::Foreground fg = color::Foreground::GREEN;

public:
  Grass(color::Foreground color) : Tile(), fg(color){};
  ~Grass(){};

  bool isOccupied() const override { return false; }
  std::string toString() override {
    return color::stylize(symbol, color::Style::DIM, fg);
  }
};

class Sand : public Tile {
private:
  static const std::string symbol;
  const color::Foreground fg = color::Foreground::WHITE;

public:
  Sand(color::Foreground color) : Tile(), fg(color){};
  ~Sand(){};

  bool isOccupied() const override { return false; }
  std::string toString() override {
    return color::stylize(symbol, color::Style::DIM, fg);
  }
};

class Water : public Tile {
private:
  static const std::string symbol;
  const color::Foreground fg = color::Foreground::BLUE;

public:
  Water(color::Foreground color) : Tile(), fg(color){};
  ~Water(){};

  bool isOccupied() const override { return true; }
  std::string toString() override {
    return color::stylize(symbol, color::Style::DIM, fg);
  }
};

#endif