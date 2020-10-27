#include "human_player.h"
#include "exceptions.h"
#include "formatting.h"
#include "move.h"
#include "place_result.h"
#include "rang.h"
#include "tile_kind.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <vector>

using namespace std;

// This method is fully implemented.
inline string &to_upper(string &str) {
  transform(str.begin(), str.end(), str.begin(), ::toupper);
  return str;
}

// return and validate move from human players
Move HumanPlayer::get_move(const Board &board,
                           const Dictionary &dictionary) const {
  while (true) {
    print_hand(cout);
    string input;
    getline(cin, input);
    while (input == "\n" || input == "") {
      getline(cin, input);
    }
    input = to_upper(input);

    if (input == "PASS") {
      return Move();
    } else if (input.substr(0, 8) == "EXCHANGE") {
      vector<TileKind> tk = parse_tiles(input);
      // if something went wrong, ans will have size 0,
      if (tk.size() != 0)
        return Move(tk);
    } else if (input.substr(0, 5) == "PLACE") {
      Move ans = parse_move(input);
      ans.column -= 1;
      ans.row -= 1;
      if (ans.kind == MoveKind::PASS)
        continue; // this is to signal something went wrong with their command
      PlaceResult pr = board.test_place(
          ans); // see if the place command can work on the board
      if (!pr.valid) {
        cout << pr.error << endl;
        continue;
      }
      bool known_words = true; // see if all words formed are legal
      for (string s : pr.words) {
        if (!dictionary.is_word(s)) {
          cout << "One of the newly formed words is ILLEGAL: " << s << endl;
          known_words = false;
          break;
        }
      }
      if (known_words)
        return ans;
    } else {
      cout << "Invalid move inputted!" << endl;
    }
  }
}

// EXCHANGE helper function E.G. EXCHANGE EEE
vector<TileKind> HumanPlayer::parse_tiles(string &letters) const {
  stringstream ss(letters);
  string temp;
  ss >> temp; // temp = "EXCHANGE"
  ss >> temp; // temp = letters to be exchanged

  // register all letters to be exchanged into map that has key of letter and
  // value of #
  map<char, size_t> ex;
  vector<TileKind> empty, exchanged;

  for (size_t i = 0; i < temp.size(); i++) {
    try {
      exchanged.push_back(tiles.lookup_tile(temp[i]));
    } catch (const out_of_range &) {
      cout << "Invalid tiles inputted: certain letters are not in your hand"
           << endl;
      return empty;
    }

    if (ex.find(temp[i]) != ex.end()) {
      ex[temp[i]] += 1;
    } else {
      ex[temp[i]] = 1;
    }
  }
  // look up all tile and see if it exist in hand and has enough letters

  for (auto i = ex.begin(); i != ex.end(); i++) {

    TileKind current = tiles.lookup_tile(i->first);
    if (tiles.count_tiles(current) < i->second) {
      cout << "Invalid tiles inputted: not enough letters in your hand" << endl;
      return empty;
    }
  }
  return exchanged;
}

// MOVE helper function, E.G. PLACE | 7 9 VO?LA
Move HumanPlayer::parse_move(string &move_string) const {
  // Preparation
  // stringstream ss to help parse thru move_String
  // temp to hold the words
  // vector placed to store cards and wildcard (whose true value is stored in
  // its member "assigned")
  // size_t row, col to store information from move_string
  // direction dir to store information from move_string
  stringstream ss(move_string);
  string temp;
  ss >> temp; // temp = "PLACE"
  vector<TileKind> placed;
  size_t row, col;
  Direction dir;

  // Direction
  ss >> temp;
  if (temp == "|") {
    dir = Direction::DOWN;
  } else if (temp == "-") {
    dir = Direction::ACROSS;
  } else {
    cout << "Invalid direction detected!";
    return Move();
  }

  // Row and Cols
  try {
    ss >> temp;
    row = stoi(temp);
    ss >> temp;
    col = stoi(temp);
  } catch (const invalid_argument &) {
    cerr << "Invalid row and col inputted" << endl;
    return Move();
  }
  // Tiles
  // string nt is temp without what ? represents but also disguised as EXCHANGE
  // vector wilds is to orderly record what ? represents
  // size_t i is index for traversing thru temp
  // size_t j is index for traversing thru wilds; max(j) + 1 should be
  // wilds.size()
  ss >> temp;
  vector<char> wilds;
  string nt = "EXCHANGE ";
  size_t i, j;
  for (i = 0; i < temp.size(); i++) {
    nt += temp[i];
    // to store what ? represents in wilds instead of storing in map
    if (temp[i] == '?') {
      i++;
      wilds.push_back(temp[i]);
    }
  }
  placed = parse_tiles(nt);
  // see if anything went wrong and return accordingly
  if (placed.size() != 0) {
    // assign the correct letter for the wildcards
    // note that there should be wilds.size() many ? in placed
    if (wilds.size() > 0) {
      j = 0;
      for (i = 0; i < placed.size(); i++) {
        if (placed[i].letter == '?') {
          placed[i].assigned = wilds[j];
          j++;
        }
        if (j == wilds.size())
          break;
      }
    }
    return Move(placed, row, col, dir);
  }

  return Move();
}

// This function is fully implemented.
void HumanPlayer::print_hand(ostream &out) const {
  const size_t tile_count = tiles.count_tiles();
  const size_t empty_tile_count = this->get_hand_size() - tile_count;
  const size_t empty_tile_width = empty_tile_count * (SQUARE_OUTER_WIDTH - 1);

  for (size_t i = 0; i < HAND_TOP_MARGIN - 2; ++i) {
    out << endl;
  }

  out << repeat(SPACE, HAND_LEFT_MARGIN) << FG_COLOR_HEADING
      << "Your Hand: " << endl
      << endl;

  // Draw top line
  out << repeat(SPACE, HAND_LEFT_MARGIN) << FG_COLOR_LINE
      << BG_COLOR_NORMAL_SQUARE;
  print_horizontal(tile_count, L_TOP_LEFT, T_DOWN, L_TOP_RIGHT, out);
  out << repeat(SPACE, empty_tile_width) << BG_COLOR_OUTSIDE_BOARD << endl;

  // Draw middle 3 lines
  for (size_t line = 0; line < SQUARE_INNER_HEIGHT; ++line) {
    out << FG_COLOR_LABEL << BG_COLOR_OUTSIDE_BOARD
        << repeat(SPACE, HAND_LEFT_MARGIN);
    for (auto it = tiles.cbegin(); it != tiles.cend(); ++it) {
      out << FG_COLOR_LINE << BG_COLOR_NORMAL_SQUARE << I_VERTICAL
          << BG_COLOR_PLAYER_HAND;

      // Print letter
      if (line == 1) {
        out << repeat(SPACE, 2) << FG_COLOR_LETTER << (char)toupper(it->letter)
            << repeat(SPACE, 2);

        // Print score in bottom right
      } else if (line == SQUARE_INNER_HEIGHT - 1) {
        out << FG_COLOR_SCORE << repeat(SPACE, SQUARE_INNER_WIDTH - 2)
            << setw(2) << it->points;

      } else {
        out << repeat(SPACE, SQUARE_INNER_WIDTH);
      }
    }
    if (tiles.count_tiles() > 0) {
      out << FG_COLOR_LINE << BG_COLOR_NORMAL_SQUARE << I_VERTICAL;
      out << repeat(SPACE, empty_tile_width) << BG_COLOR_OUTSIDE_BOARD << endl;
    }
  }

  // Draw bottom line
  out << repeat(SPACE, HAND_LEFT_MARGIN) << FG_COLOR_LINE
      << BG_COLOR_NORMAL_SQUARE;
  print_horizontal(tile_count, L_BOTTOM_LEFT, T_UP, L_BOTTOM_RIGHT, out);
  out << repeat(SPACE, empty_tile_width) << rang::style::reset << endl;
}
