#include "scrabble.h"
#include "formatting.h"
#include <iomanip>
#include <iostream>
#include <map>

using namespace std;

// Given to you. this does not need to be changed
Scrabble::Scrabble(const ScrabbleConfig &config)
    : hand_size(config.hand_size),
      minimum_word_length(config.minimum_word_length),
      tile_bag(TileBag::read(config.tile_bag_file_path, config.seed)),
      board(Board::read(config.board_file_path)),
      dictionary(Dictionary::read(config.dictionary_file_path)) {}

/*
3
playerA
playerB
playerC
*/
void Scrabble::add_players() {
  cout << "How many players? " << endl;
  int num;
  string name;
  cin >> num;
  for (int i = 0; i < num; i++) {
    cout << "Please enter your name, player " << (i + 1) << ": " << endl;
    cin >> name;
    players.push_back(make_shared<HumanPlayer>(name, 7));
  }
}

// Game Loop should cycle through players and get and execute that players move
// until the game is over.
void Scrabble::game_loop() {
  int loop = 0;
  size_t true_index;
  bool cont = true;
  size_t all_pass = 0;
  string enter;

  // populate player with cards!
  for (size_t i = 0; i < players.size(); i++) {
    vector<TileKind> tk =
        tile_bag.remove_random_tiles(players[i]->get_hand_size());
    for (TileKind k : tk)
      players[i]->add_tile(k);
  }

  // while the game continues
  while (cont) {
    board.print(cout);

    true_index = loop % players.size();

    // to ensure we have a valid move; move kind can only be pass, exchange or
    // place
    Move m = players[true_index]->get_move(board, dictionary);

    

    if (m.kind == MoveKind::PASS) {
      // 0. update all_pass streak
      //      if everyone passed, it will eventually reach the size of the group
      //      if any single person did NOT pass, the streak goes back to 0
      // 1. communicate
      all_pass += 1;

      cout << "You passed successfully!" << endl;
      cout << "Your current score: " << SCORE_COLOR
           << players[true_index]->get_points() << rang::style::reset << endl;
      do {
        cout << "Press [enter] to continue." << endl;
        getline(cin, enter);
      } while (enter != "");
    } else if (m.kind == MoveKind::EXCHANGE) {
      // 0. kill all_pass streak
      // 1. check if there're enough tiles from bag to be exchanged
      // 2. if there are enough from bag, update on player's card;
      //    if not, continue and get new move without updating loop
      // 3. communicate
      all_pass = 0;

      for (TileKind tk: m.tiles) {
        tile_bag.add_tile(tk);
      }
      players[true_index]->remove_tiles(m.tiles);
      players[true_index]->add_tiles(tile_bag.remove_random_tiles(m.tiles.size()));
      
      cout << "You exchanged successfully!" << endl;
      cout << "Your current score: " << SCORE_COLOR
           << players[true_index]->get_points() << rang::style::reset << endl;
      do {
        cout << "Press [enter] to continue." << endl;
        getline(cin, enter);
      } while (enter != "");
    } else {
      // 0. kill pass streak
      // 1. place the tiles down onto board
      // 2. update player's tiles (remember to check if tile_bag has enough
      // tiles)
      // 3. check for BONUS POINTS and update player's points
      // 4. communicate
      all_pass = 0;

      PlaceResult pr = board.place(m);
      int sum = pr.points;

      players[true_index]->remove_tiles(m.tiles);
      players[true_index]->add_tiles(tile_bag.remove_random_tiles(
          min(m.tiles.size(), tile_bag.count_tiles())));

      if (m.tiles.size() == players[true_index]->get_hand_size())
        sum += 50;
      players[true_index]->add_points(sum);

      cout << "You gained " << SCORE_COLOR << sum << rang::style::reset
           << " points!" << endl;
      cout << "Your current score: " << SCORE_COLOR
           << players[true_index]->get_points() << rang::style::reset << endl;
      do {
        cout << "Press [enter] to continue." << endl;
        getline(cin, enter);
      } while (enter != "");
    }
    // update on loop variable and see if it's time to END the game and this
    // function
    loop++;
    if (players[true_index]->count_tiles() == 0)
      cont = false;
    if (all_pass == players.size())
      cont = false;
  }

  // Useful cout expressions with fancy colors. Expressions in curly braces,
  // indicate values you supply.
  // cout << "You gained " << SCORE_COLOR << {points} << rang::style::reset << "
  // points!" << endl;
  // cout << "Your current score: " << SCORE_COLOR << {points} <<
  // rang::style::reset << endl;
  // cout << endl << "Press [enter] to continue.";
}

// Performs final score subtraction. Players lose points for each tile in their
// hand. The player who cleared their hand receives all the points lost by the
// other players.
void Scrabble::final_subtraction(vector<shared_ptr<Player>> &plrs) {
  // TODO: implement this method.
  // Do not change the method signature.
  int total_subtracted = 0;
  int winner = -1;
  for (size_t i = 0; i < plrs.size(); i++) {
    int subtracted = plrs[i]->get_hand_value();
    plrs[i]->subtract_points(subtracted);
    total_subtracted += subtracted;
    if (plrs[i]->count_tiles() == 0)
      winner = i;
  }

  if (winner != -1)
    plrs[winner]->add_points(total_subtracted);
}

// You should not need to change this function.
void Scrabble::print_result() {
  // Determine highest score
  size_t max_points = 0;
  for (auto player : this->players) {
    if (player->get_points() > max_points) {
      max_points = player->get_points();
    }
  }

  // Determine the winner(s) indexes
  vector<shared_ptr<Player>> winners;
  for (auto player : this->players) {
    if (player->get_points() >= max_points) {
      winners.push_back(player);
    }
  }

  cout << (winners.size() == 1 ? "Winner:" : "Winners: ");
  for (auto player : winners) {
    cout << SPACE << PLAYER_NAME_COLOR << player->get_name();
  }
  cout << rang::style::reset << endl;

  // now print score table
  cout << "Scores: " << endl;
  cout << "---------------------------------" << endl;

  // Justify all integers printed to have the same amount of character as the
  // high score, left-padding with spaces
  cout << setw(static_cast<uint32_t>(floor(log10(max_points) + 1)));

  for (auto player : this->players) {
    cout << SCORE_COLOR << player->get_points() << rang::style::reset << " | "
         << PLAYER_NAME_COLOR << player->get_name() << rang::style::reset
         << endl;
  }
}

// You should not need to change this.
void Scrabble::main() {
  add_players();
  game_loop();
  final_subtraction(this->players);
  print_result();
}
