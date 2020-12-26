#include "player.h"

using namespace std;

void Player::add_points(size_t points) { this->points += points; }

void Player::subtract_points(size_t points) {
  if (this->points < points)
    this->points = 0;
  else
    this->points -= points;
}

size_t Player::get_points() const { return this->points; }

const string &Player::get_name() const { return this->name; }

size_t Player::count_tiles() const { return tiles.count_tiles(); }

void Player::remove_tiles(const vector<TileKind> &tiles) {
  for (size_t i = 0; i < tiles.size(); i++) {
    this->tiles.remove_tile(tiles[i]);
  }
}

void Player::add_tiles(const vector<TileKind> &tiles) {
  for (size_t i = 0; i < tiles.size(); i++) {
    this->tiles.add_tile(tiles[i]);
  }
}

bool Player::has_tile(TileKind tile) {
  // should return true if no error thrown
  try {
    tiles.lookup_tile(tile.letter);
  } catch (const out_of_range &) {
    return false;
  }
  return true;
}

unsigned int Player::get_hand_value() const { return tiles.total_points(); }

size_t Player::get_hand_size() const { return hand_size; }