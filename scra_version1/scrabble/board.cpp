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

/*
  NOTE:
    d = double word bonus; t = triple word bonus
    2 = double letter bonus; 3 = triple letter bonus
*/
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
      } else { //line[i] == '.'
        row.push_back(BoardSquare(1, 1));
      }
    }
    board.squares.push_back(row);
  }

  return board;
}

size_t Board::get_move_index() const { return this->move_index; }

/*
  TODO:
    test_place function for edge-case purpose

  CHECK:
    1. did not use start tile for first move
    2. no input for first move
*/
PlaceResult Board::test_place_firstmove(const Move& move) const{
  unsigned int sum = 0;
  string main_word = "";
  vector<string> formed;
  vector<int> m;

    // no input
    if (move.tiles.size() == 1)
      return PlaceResult("NOWORDS: Inputting single tile for first move");


    // start tile not used
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

/*
VARIABLES OVERVIEW:
unsigned sum: 
      total point for this move
unsigned prev_letter_sum: 
      main_word's letter point
string main_word: 
      the word placing onto board right now
string adjacent: 
      temp str for every adjacent word to main_word
vector <string> formed: 
      store all adjacent words and main_word as last element
vector <int>  m:
      stores all new tiles' word multiplier
bool	adj:
      help check if any adjacent words are present
int	current_multi:
      check if any multiplier exist for the currently placed tile
*/

/*
  TODO:
    understand whether move is valid or not and keep the board un-updated

  CHECK:
    0. keep in mind of out of bound or no neighbor erros
    1. if some are occupied add to the actual word
    2. remember to pick up words that are adjacent to what is added
    3. check if ending position is now out of bound
    4. now go thru all newly formed words and compare them against dictionary

  NOTE:
    - sum does not add prev_letter_sum until the end
   	- sum should add adjacent word's total points * multiplier on
   	- new tiles and new tile point * letter multiplier
*/

PlaceResult Board::test_place(const Move &move) const {
  unsigned int sum = 0, prev_letter_sum = 0, total_adj = 0, adj_sum,
               current_multi;
  string main_word = "", adjacent;
  vector<string> formed;
  vector<int> m;
  bool adj = false;

  // NO_START (didn't use START tile for first move)
  // NO_WORDS (input single letter for first move)

  if(move_index == 0)
	  return test_place_firstmove(move);
  
  // OCCUPIED (check for start position which must be both empty and within bound)

  if (!is_in_bounds(Position(move.row, move.column)) ||
      at(Position(move.row, move.column)).has_tile()) {
    std::cerr << "line 476";
    return PlaceResult("Starting position out of bound or empty");
  }

  // OUT_OF_BOUND; NO_NEIGHBORS

  // first check if our new word is a continuation of previously existed word
  // then pay attention to adjacent words(- up/down | left/right)
  // when we arrive at last letter of our main word, see if end is out of bound

  // check to see if there's any tiles present that we need to consider before our current tile

  Position p (move.row, move.column);
  while(in_bounds_and_has_tile(p.translate(move.direction, -1))){
    adj = true;
    p = p.translate(move.direction, -1);
    prev_letter_sum += at(p).get_tile_kind().points;

    if(at(p).get_tile_kind().letter == '?') 
      main_word = at(p).get_tile_kind().assigned + main_word;
    else 
      main_word = at(p).get_tile_kind().letter + main_word;
  }

  // now process tiles starting from our current tile

  p.row = move.row; p.column = move.column;
  for(size_t i = 0; i < move.tiles.size(); i++){
    if(!is_in_bounds(p)){
      std::cerr << "line 498";return PlaceResult("Certain letters out of bound");
    } 

    if(at(p).has_tile()){
      adj = true;
      
      if(at(p).get_tile_kind().letter == '?') 
        main_word += at(p).get_tile_kind().assigned;
      else 
        main_word += at(p).get_tile_kind().letter;
      
      prev_letter_sum += at(p).get_tile_kind().points;
      i--;

    }else{
      adjacent = "";
      adj_sum = 0;
      
      if(move.tiles[i].letter == '?') 
        main_word += move.tiles[i].assigned;
      else 
        main_word += move.tiles[i].letter;

      current_multi = squares[p.row][p.column].word_multiplier;
      sum += move.tiles[i].points * squares[p.row][p.column].letter_multiplier;
      m.push_back(current_multi);

      Position prev_adj = p;
      Direction d;

      if(move.direction == Direction::ACROSS) 
        d = Direction::DOWN;
      else 
        d = Direction::ACROSS;

      // look to prev for the adjacent tiles

      while(in_bounds_and_has_tile(prev_adj.translate(d, -1))){
        adj = true;
        prev_adj = prev_adj.translate(d, -1);

        if(at(prev_adj).get_tile_kind().letter !='?') 
          adjacent = at(prev_adj).get_tile_kind().letter + adjacent;
        else 
          adjacent = at(prev_adj).get_tile_kind().assigned + adjacent;

        adj_sum += at(prev_adj).get_tile_kind().points;
      }

      // current tile

      adjacent += main_word[main_word.size() - 1];
      adj_sum += move.tiles[i].points * squares[p.row][p.column].letter_multiplier;
      prev_adj = p;

      // adjacent tiles

      while(in_bounds_and_has_tile(prev_adj.translate(d, 1))){
        adj = true;

        prev_adj = prev_adj.translate(d, 1);
        if(at(prev_adj).get_tile_kind().letter !='?') 
          adjacent += at(prev_adj).get_tile_kind().letter;
        else 
          adjacent += at(prev_adj).get_tile_kind().assigned;

        adj_sum += at(prev_adj).get_tile_kind().points;
      }

      if(adjacent.size() > 1){
        formed.push_back(adjacent);
        total_adj += adj_sum * current_multi;
      }
    } // end of else statement that says we DON'T have a tile at our current position

    p = p.translate(move.direction);

  }// end of for loop with all the move tiles considered

  // check if any adjacent tiles are followed by main_words

  while(in_bounds_and_has_tile(p)){
    adj = true;

    if(at(p).get_tile_kind().letter !='?') 
      main_word += at(p).get_tile_kind().letter;
    else 
      main_word += at(p).get_tile_kind().assigned;

    prev_letter_sum += at(p).get_tile_kind().points; 
    p = p.translate(move.direction, 1);
  }

  if(!adj){
    std::cerr << "line 560";
    return PlaceResult("No adjacent words found!");
  }

  // finish sum calculation: adjacent + main_word

  sum += prev_letter_sum;

  for (int l : m){
    sum *= l;
  }
       
  sum += total_adj; 

  formed.push_back(main_word);   
    
  return PlaceResult(formed, sum); 
}


/*
TODO: 
  place a certain move onto board

NOTE:
  - check for validity of move and if player PASSED (did nothing)
*/
PlaceResult Board::place(const Move &move) {
  PlaceResult ans = test_place(move);
  
  // return if invalid
  if (ans.valid) {

    // check if player PASSED
    if (ans.words.size() == 0 && ans.points == 0)
      return ans;

    // place main_word onto board
    Position temp(move.row, move.column);
    for (size_t i = 0; i < move.tiles.size(); i++) {
      // update on empty squares
      if (at(temp).has_tile())
        i--;
      else 
        squares[temp.row][temp.column].set_tile_kind(move.tiles[i]);

      // update temp's location regardless
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
