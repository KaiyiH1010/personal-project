#include "board.h"
#include "board_square.h"
#include "exceptions.h"
#include "formatting.h"
#include <cmath>
#include <fstream>
#include <iomanip>

using namespace std;

bool Board::Position::operator==(const Board::Position &other) const {
  return this->row == other.row && this->column == other.column;
}

bool Board::Position::operator!=(const Board::Position &other) const {
  return this->row != other.row || this->column != other.column;
}

Board::Position Board::Position::translate(Direction direction) const {
  return this->translate(direction, 1);
}

Board::Position Board::Position::translate(Direction direction,
                                           ssize_t distance) const {
  if (direction == Direction::DOWN) {
    return Board::Position(this->row + distance, this->column);
  } else {
    return Board::Position(this->row, this->column + distance);
  }
}

Board Board::read(const string &file_path) {
  ifstream file(file_path);
  if (!file) {
    throw FileException("cannot open board file!");
  }

  size_t rows;
  size_t columns;
  size_t starting_row;
  size_t starting_column;
  file >> rows >> columns >> starting_row >> starting_column;
  Board board(rows, columns, starting_row, starting_column);

  // TODO: read thru each line of the board
  // d = double word bonus; t = triple word bonus
  // 2 = double letter bonus; 3 = triple letter bonus
  string line;
  while (!file.fail()) {
    file >> line;
    vector<BoardSquare> row;

    for (size_t i = 0; i < line.size(); i++) {
      if (line[i] == 'd') {
        row.push_back(BoardSquare(1, 2));
      } else if (line[i] == 't') {
        row.push_back(BoardSquare(1, 3));
      } else if (line[i] == '2') {
        row.push_back(BoardSquare(2, 1));
      } else if (line[i] == '3') {
        row.push_back(BoardSquare(3, 1));
      } else { // line[i] == '.'
        row.push_back(BoardSquare(1, 1));
      }
    }
    board.squares.push_back(row);
  }

  return board;
}

size_t Board::get_move_index() const { return this->move_index; }


PlaceResult Board::test_place_firstmove(const Move& move) const{
  unsigned int sum = 0;
  string main_word = "";
  vector<string> formed;
  vector<int> m;

  // TARGETS: NO_START (didn't use START tile for first move); NO_WORDS(input
  // single letter for first move)
    // NOWORDS
    if (move.tiles.size() == 1)
      return PlaceResult("NOWORDS: Inputting single tile for first move");
    // NOSTART
    if (move.row == start.row && move.direction == Direction::ACROSS) {
      if (!(move.column <= start.column) &&
          !(start.column <= (move.column + move.tiles.size()))) {
        return PlaceResult(
            "NOSTART: did NOT use starting tile for first move: case 1");
      }
    } else if (move.column == start.column &&
               move.direction == Direction::DOWN) {
      if (!(move.row <= start.row) &&
          !(start.row <= (move.row + move.tiles.size()))) {
        return PlaceResult(
            "NOSTART: did NOT use starting tile for first move: case 2");
      }
    } else {
      return PlaceResult(
          "NOSTART: did NOT use starting tile for first move: case 3");
    }
    Position moving(move.row, move.column);
    for (size_t i = 0; i < move.tiles.size(); i++) {
      if (i != 0) {
        if (move.direction == Direction::ACROSS) {
          if (moving.column + 1 < columns)
            moving.column += 1;
          else
            return PlaceResult("Certain letter out of bound!");
        } else {
          if (moving.row + 1 < rows)
            moving.row += 1;
          else
            return PlaceResult("Certain letter out of bound!");
        }
      }
      m.push_back(squares[moving.row][moving.column].word_multiplier);
      sum += move.tiles[i].points *
             squares[moving.row][moving.column].letter_multiplier;
      main_word += move.tiles[i].letter;
    }
    formed.push_back(main_word);
    for (int l : m)
      sum *= l;
    return PlaceResult(formed, sum);
  
	
}

// VARIABLES:
// unsigned int sum is to calculate the total point for this move
// unsigned int prev_letter_sum is to keep track of main_word's letter point
// that are placed before this turn if there are any word multiplier present for new
// tiles
// 			NOTE: sum does not add prev_letter_sum until the end;
// 				  sum should add adjacent word's total points * multiplier on
// 				  new tiles and new tile point * letter multiplier
// string main_word is the word we are placing onto board right now
// 		  adjacent is temp hold for every adjacent word we touch
// vector <string> formed store all words with adjacent words in the front and
// main_word at the end
// vector <int>  m stores all word multiplier on new tiles
// bool	adj is to help check if any adjacent words are present
//	int	current_multi is to check if any multiplier exist for the current
//newly placed tile we are looking at

// THOUGHT PROCESS for TARGETS: OUT_OF_BOUND; NO_NEIGHBORS:
// 1. if some are occupied add to the actual word
// 2. remember to pick up words that are adjacent to what is added
// 3. check if ending position is now out of bound
// 4. now go thru all newly formed words and compare them against dictionary
PlaceResult Board::test_place(const Move &move) const {
  unsigned int sum = 0, prev_letter_sum = 0, total_adj = 0, adj_sum,
               current_multi;
  string main_word = "", adjacent;
  vector<string> formed;
  vector<int> m;
  bool adj = false;
  // TARGETS: NO_START (didn't use START tile for first move); NO_WORDS(input
  // single letter for first move)
  if(move_index == 0)
	return test_place_firstmove(move);
  
  // TARGETS: OCCUPIED
  // check for start position which must be both empty and within bound
  if (!is_in_bounds(Position(move.row, move.column)) ||
      at(Position(move.row, move.column)).has_tile()) {
    return PlaceResult("Starting position out of bound or empty");
  }

  // TARGETS: OUT_OF_BOUND; NO_NEIGHBORS

  // first check if our new word is a continuation of previously existed word
  // then pay attention to adjacent words(- up/down | left/right)
  // when we arrive at last letter of our main word, see if end is out of bound
  if (move.direction == Direction::ACROSS) { 		// dir "-"
    // BEFORE NEW TILE
    // find the right starting point for the word we are forming
    bool prev = true;
    if(move.column < 1)
      prev = false;
    Position previous(move.row, move.column - 1);
    while (prev && at(previous).has_tile()) {
      previous.column -= 1;
    }
    // now if the true starting pt is not provided by move
    // update the previous letters and scores onto our main word and sum
    // remember to add the assigned letter when it's a wildcard '?'
    if (previous.column != move.column - 1) {
      adj = true;
      previous.column += 1;
      while (previous.column != move.column) {
        if (at(previous).get_tile_kind().letter == '?') {
          main_word += tolower(at(previous).get_tile_kind().assigned);
        } else {
          main_word += at(previous).get_tile_kind().letter;
        }
        prev_letter_sum += at(previous).get_tile_kind().points;
        previous.column += 1;
      }
    }
    // DURING NEW TILES (and some previously placed tiles (forming main_word and
    // possibly adjacent))

    // we have processed all previous letters from move's initial starting
    // position
    // only process adjacent words from now on
    Position moving(move.row, move.column - 1);
    for (size_t i = 0; i < move.tiles.size(); i++) {
      // check constantly if tiles is out of bound; ONLY advance moving if it's not
      if (moving.column + 1 < columns)
        moving = moving.translate(Direction::ACROSS);
      else
        return PlaceResult("Certain letters out of bound");
      // UPDATE MAIN_WORD AND SUM/PREV_SUM
      if (at(moving).has_tile()) {
        adj = true;
        // if there are previously placed tile encountered, do NOT update i and
        // ignore rest of the code
        if (at(moving).get_tile_kind().letter == '?')
          main_word += tolower(at(moving).get_tile_kind().assigned);
        else
          main_word += at(moving).get_tile_kind().letter;
        prev_letter_sum += at(moving).get_tile_kind().points;
        i--;
        continue;
      }
      // from now on it is basically the else statement for when moving has NO
      // tile placed there before
      if (move.tiles[i].letter == '?')
        main_word += tolower(move.tiles[i].assigned);
      else
        main_word += move.tiles[i].letter;

      current_multi = squares[moving.row][moving.column].word_multiplier;
      m.push_back(current_multi);

      sum += move.tiles[i].points *
             squares[moving.row][moving.column].letter_multiplier;
      // CHECK FOR ADJACENT
      // see if there's adjacent work either above or below the current word;
      // reset adjacent to empty because for each tile there may be a new
      // adjacent
      adjacent = "";
      adj_sum = 0;
      bool up = true, low = true;
      if(moving.row < 1)
        up = false;
      if(moving.row  > rows-2)
        low = false;
      Position upper(moving.row - 1, moving.column);
      Position lower(moving.row + 1, moving.column);
      // if there are tiles above: get letters for adjacent word until moving
      // position+1 (moving included)
      if (up && at(upper).has_tile()) {
        adj = true;
        while (at(upper).has_tile())
          upper.row -= 1;
        if (upper.row != moving.row - 1) {
          upper.row += 1;
          while (upper.row != moving.row) {
            if (at(upper).get_tile_kind().letter == '?')
              adjacent += tolower(at(upper).get_tile_kind().assigned);
            else
              adjacent += at(upper).get_tile_kind().letter;
            adj_sum += at(upper).get_tile_kind().points;
            upper.row += 1;
          }
          // finally, add letter at moving onto adjacent and update adj_sum
          adjacent += main_word[main_word.size() - 1];
          adj_sum += move.tiles[i].points *
                     squares[moving.row][moving.column].letter_multiplier;
        }
      }
      // start to get letters from moving position till no tile
      if (low && at(lower).has_tile()) {
        // if there is no upper, add the letter at moving to adjacent
        if (adjacent == "") {
          adjacent += main_word[main_word.size() - 1];
          adj_sum += move.tiles[i].points *
                     squares[moving.row][moving.column].letter_multiplier;
        }
        adj = true;
        while (at(lower).has_tile())
          lower.row += 1; // this loop ends when the lower has no tile
        if (lower.row != moving.row + 1) {
          Position temp(moving.row + 1, moving.column);
          while (lower.row != temp.row) {
            if (at(temp).get_tile_kind().letter == '?')
              adjacent += tolower(at(temp).get_tile_kind().assigned);
            else
              adjacent += at(temp).get_tile_kind().letter;
            adj_sum += at(temp).get_tile_kind().points;
            temp.row += 1;
          }
        }
      }
      if (adjacent != "") {
        formed.push_back(adjacent);
        total_adj += adj_sum * current_multi;
      }
    } // end of for loop for going thru all move's tile
    // move is at the last newly placed tile's position, direction is still
    // ACROSS
    // update on main_word and prev_letter_sum
    while (moving.column + 1 < columns) {
      moving = moving.translate(Direction::ACROSS);
      if (at(moving).has_tile()) {
        adj = true;
        if (at(moving).get_tile_kind().letter == '?')
          main_word += tolower(at(moving).get_tile_kind().assigned);
        else
          main_word += at(moving).get_tile_kind().letter;
        prev_letter_sum += at(moving).get_tile_kind().points;
      } else {
        break;
      }
    }
  } else { // dir "|"
    bool prev = true;
    if(move.row < 1)
      prev = false;
    Position previous(move.row - 1, move.column);
    while (prev && at(previous).has_tile()){
      previous.row -= 1;
    }
    if (previous.row != move.row - 1) {
      adj = true;
      previous.row += 1;
      while (previous.row != move.row) {
        if (at(previous).get_tile_kind().letter == '?')
          main_word += tolower(at(previous).get_tile_kind().assigned);
        else
          main_word += at(previous).get_tile_kind().letter;
        prev_letter_sum += at(previous).get_tile_kind().points;
        previous.row += 1;
      }
    }
    Position moving(move.row - 1, move.column);
    for (size_t i = 0; i < move.tiles.size(); i++) {
      if (moving.row + 1 < rows)
        moving = moving.translate(Direction::DOWN);
      else
        return PlaceResult("Certain letters out of bound");
      if (at(moving).has_tile()) {
        adj = true;
        if (at(moving).get_tile_kind().letter == '?')
          main_word += tolower(at(moving).get_tile_kind().assigned);
        else
          main_word += at(moving).get_tile_kind().letter;
        prev_letter_sum += at(moving).get_tile_kind().points;
        i--;
        continue;
      }
      if (move.tiles[i].letter == '?')
        main_word += tolower(move.tiles[i].assigned);
      else
        main_word += move.tiles[i].letter;
      current_multi = squares[moving.row][moving.column].word_multiplier;
      m.push_back(current_multi);
      
      sum += move.tiles[i].points *
             squares[moving.row][moving.column].letter_multiplier;

      adjacent = "";
      adj_sum = 0;
      bool l = true, r = true;
      if(moving.column < 1)
        l = false;
      if(moving.column > columns-2)
        r = false;
      Position left(moving.row, moving.column - 1);
      Position right(moving.row, moving.column + 1);
      if (l && at(left).has_tile()) {
        adj = true;
        while (at(left).has_tile()) {
          left.column -= 1;
        }
        if (left.column != move.column - 1) {
          left.column += 1;
          while (left.column != moving.column) {
            if (at(left).get_tile_kind().letter == '?') {
              adjacent += tolower(at(left).get_tile_kind().assigned);
            } else {
              adjacent += at(left).get_tile_kind().letter;
            }
            adj_sum += at(left).get_tile_kind().points;
            left.column += 1;
          }
          adjacent += main_word[main_word.size() - 1];
          adj_sum += move.tiles[i].points *
                     squares[moving.row][moving.column].letter_multiplier;
        }
      }
      if (r && at(right).has_tile()) {
        if (adjacent == "") {
          adjacent += main_word[main_word.size() - 1];
          adj_sum += move.tiles[i].points *
                     squares[moving.row][moving.column].letter_multiplier;
        }
        adj = true;
        while (at(right).has_tile())
          right.column += 1;
        if (right.column != move.column + 1) {
          Position temp(moving.row, moving.column+1);
          while (right.column != temp.column) {
            if (at(temp).get_tile_kind().letter == '?') {
              adjacent += tolower(at(temp).get_tile_kind().assigned);
            } else {
              adjacent += at(temp).get_tile_kind().letter;
            }
            adj_sum += at(temp).get_tile_kind().points;
            temp.column += 1;
          }
        }
      }
      if (adjacent != "") {
        formed.push_back(adjacent);
        total_adj += adj_sum * current_multi;
      }
    } // end of for loop for going thru all move's tile
    while (moving.row + 1 < rows) {
      moving = moving.translate(Direction::DOWN);
      if (at(moving).has_tile()) {
        adj = true;
        if (at(moving).get_tile_kind().letter == '?') {
          main_word += tolower(at(moving).get_tile_kind().assigned);
        } else {
          main_word += at(moving).get_tile_kind().letter;
        }
        prev_letter_sum += at(moving).get_tile_kind().points;
      } else {
        break;
      }
    }
  } // end of big if else statement for move direction
  if (!adj)
    return PlaceResult("No adjacent words found!");

  sum += prev_letter_sum;
  for (int l : m)
    sum *= l;       // sum won't be multiplied if m is empty!
  sum += total_adj; // add the effective adjacent score onto sum too

  if (formed.size() > 0 && main_word.size() > 1)
    formed.push_back(main_word);   // push the main word into formed as well
  if(main_word.size() == 1){
    sum -= move.tiles[0].points * m[0];
    unsigned short lm = squares[move.row][move.column].letter_multiplier;
    if(lm > 1)
      sum -= move.tiles[0].points * lm;
  }
    
  return PlaceResult(formed, sum); 
}

PlaceResult Board::place(const Move &move) {
  PlaceResult ans = test_place(move);
  // only change board if it's valid
  if (ans.valid) {
    // this only happens when player PASSED
    if (ans.words.size() == 0 && ans.points == 0)
      return ans;

    // place each letter of main_word onto board with positions indicated by
    // move and board
    Position temp(move.row, move.column);
    for (size_t i = 0; i < move.tiles.size(); i++) {
      // do nothing if there's a tile there already
      // update on square's tile otherwise
      if (at(temp).has_tile()) {
        i--;
      } else {
        squares[temp.row][temp.column].set_tile_kind(move.tiles[i]);
      }
      // whether or not we update board's tile, we update temp's location
      if (move.direction == Direction::ACROSS)
        temp.column += 1;
      else
        temp.row += 1;
    }

    // update on move_index
    move_index++;
  }
  return ans;
}

// The rest of this file is provided for you. No need to make changes.

BoardSquare &Board::at(const Board::Position &position) {
  return this->squares.at(position.row).at(position.column);
}

const BoardSquare &Board::at(const Board::Position &position) const {
  return this->squares.at(position.row).at(position.column);
}

bool Board::is_in_bounds(const Board::Position &position) const {
  return position.row < this->rows && position.column < this->columns;
}

bool Board::in_bounds_and_has_tile(const Position &position) const {
  return is_in_bounds(position) && at(position).has_tile();
}

void Board::print(ostream &out) const {
  // Draw horizontal number labels
  for (size_t i = 0; i < BOARD_TOP_MARGIN - 2; ++i) {
    out << std::endl;
  }
  out << FG_COLOR_LABEL << repeat(SPACE, BOARD_LEFT_MARGIN);
  const size_t right_number_space = (SQUARE_OUTER_WIDTH - 3) / 2;
  const size_t left_number_space =
      (SQUARE_OUTER_WIDTH - 3) - right_number_space;
  for (size_t column = 0; column < this->columns; ++column) {
    out << repeat(SPACE, left_number_space) << std::setw(2) << column + 1
        << repeat(SPACE, right_number_space);
  }
  out << std::endl;

  // Draw top line
  out << repeat(SPACE, BOARD_LEFT_MARGIN);
  print_horizontal(this->columns, L_TOP_LEFT, T_DOWN, L_TOP_RIGHT, out);
  out << endl;

  // Draw inner board
  for (size_t row = 0; row < this->rows; ++row) {
    if (row > 0) {
      out << repeat(SPACE, BOARD_LEFT_MARGIN);
      print_horizontal(this->columns, T_RIGHT, PLUS, T_LEFT, out);
      out << endl;
    }

    // Draw insides of squares
    for (size_t line = 0; line < SQUARE_INNER_HEIGHT; ++line) {
      out << FG_COLOR_LABEL << BG_COLOR_OUTSIDE_BOARD;

      // Output column number of left padding
      if (line == 1) {
        out << repeat(SPACE, BOARD_LEFT_MARGIN - 3);
        out << std::setw(2) << row + 1;
        out << SPACE;
      } else {
        out << repeat(SPACE, BOARD_LEFT_MARGIN);
      }

      // Iterate columns
      for (size_t column = 0; column < this->columns; ++column) {
        out << FG_COLOR_LINE << BG_COLOR_NORMAL_SQUARE << I_VERTICAL;
        const BoardSquare &square = this->squares.at(row).at(column);
        bool is_start = this->start.row == row && this->start.column == column;

        // Figure out background color
        if (square.word_multiplier == 2) {
          out << BG_COLOR_WORD_MULTIPLIER_2X;
        } else if (square.word_multiplier == 3) {
          out << BG_COLOR_WORD_MULTIPLIER_3X;
        } else if (square.letter_multiplier == 2) {
          out << BG_COLOR_LETTER_MULTIPLIER_2X;
        } else if (square.letter_multiplier == 3) {
          out << BG_COLOR_LETTER_MULTIPLIER_3X;
        } else if (is_start) {
          out << BG_COLOR_START_SQUARE;
        }

        // Text
        if (line == 0 && is_start) {
          out << "  \u2605  ";
        } else if (line == 0 && square.word_multiplier > 1) {
          out << FG_COLOR_MULTIPLIER << repeat(SPACE, SQUARE_INNER_WIDTH - 2)
              << 'W' << std::setw(1) << square.word_multiplier;
        } else if (line == 0 && square.letter_multiplier > 1) {
          out << FG_COLOR_MULTIPLIER << repeat(SPACE, SQUARE_INNER_WIDTH - 2)
              << 'L' << std::setw(1) << square.letter_multiplier;
        } else if (line == 1 && square.has_tile()) {
          char l = square.get_tile_kind().letter == TileKind::BLANK_LETTER
                       ? square.get_tile_kind().assigned
                       : ' ';
          out << repeat(SPACE, 2) << FG_COLOR_LETTER
              << square.get_tile_kind().letter << l << repeat(SPACE, 1);
        } else if (line == SQUARE_INNER_HEIGHT - 1 && square.has_tile()) {
          out << repeat(SPACE, SQUARE_INNER_WIDTH - 1) << FG_COLOR_SCORE
              << square.get_points();
        } else {
          out << repeat(SPACE, SQUARE_INNER_WIDTH);
        }
      }

      // Add vertical line
      out << FG_COLOR_LINE << BG_COLOR_NORMAL_SQUARE << I_VERTICAL
          << BG_COLOR_OUTSIDE_BOARD << std::endl;
    }
  }

  // Draw bottom line
  out << repeat(SPACE, BOARD_LEFT_MARGIN);
  print_horizontal(this->columns, L_BOTTOM_LEFT, T_UP, L_BOTTOM_RIGHT, out);
  out << endl << rang::style::reset << std::endl;
}
