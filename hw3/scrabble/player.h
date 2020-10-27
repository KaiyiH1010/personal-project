#ifndef PLAYER_H
#define PLAYER_H

#include "move.h"
#include "board.h"
#include "dictionary.h"
#include "tile_collection.h"
#include <string>
#include <vector>
#include <algorithm>


class Player {
public:
    Player(const std::string& name, size_t hand_size) // Used for testing
        : name(name)
        , hand_size(hand_size)
        , points(0) {}
    virtual ~Player() {};

    // Adds points to player's score
    void add_points(size_t points);

    // Subtracts points from player's score
    void subtract_points(size_t points);

    size_t get_points() const;

    const std::string& get_name() const;

    // Returns a VALID move that can be executed (can be placed, exchanged, or passed).
    virtual Move get_move(const Board& board, const Dictionary& d) const = 0;

    // Returns the number of tiles in a player's hand.
    size_t count_tiles() const;

    // Removes tiles from player's hand.
    void remove_tiles(const std::vector<TileKind>& tiles);

    // Adds tiles to player's hand.
    void add_tiles(const std::vector<TileKind>& tiles);

    // Checks if player has a matching tile.
    bool has_tile(TileKind tile);

    // Returns the total points of all tiles in the players hand.
    unsigned int get_hand_value() const;

    size_t get_hand_size() const;
    void add_tile(TileKind tk){
        tiles.add_tile(tk);
    }

    void remove_tile(TileKind tk){
        tiles.remove_tile(tk);
    }
 //   size_t get_card_num() const{ return cards; }
 //   void lose_cards(int l){ cards -= l; }

protected:
    // TODO: add any protected data members or functions here
    TileCollection tiles;

    
/*    TileCollection vector_into_collection(const std::vector<TileKind>& tiles){
        TileCollection ans;
        for(size_t i = 0; i < tiles.size(); i ++){
            ans.add_tile(tiles[i]);
        }
        return ans;
    }
  */  
private:
    std::string name;
    size_t hand_size;
    size_t points;

    // TODO: add any private data members or functions here
    // size_t cards;
};

#endif
