#include "tile.hpp"

const std::string Grass::symbol = "░";
const std::string Sand::symbol = "▒";
const std::string Water::symbol = "≈";

std::shared_ptr<Tile> Tile::build(int tile_id) {
  switch (TileType(tile_id)) {
  case TileType::Grass:
    return std::make_shared<Grass>(Grass(color::Foreground::GREEN));
  case TileType::Sand:
    return std::make_shared<Sand>(Sand(color::Foreground::BRIGHT_WHITE));
  case TileType::Water:
    return std::make_shared<Water>(Water(color::Foreground::BLUE));
  default:
    throw new std::runtime_error("Unknown tile_id: " + std::to_string(tile_id));
  };
};