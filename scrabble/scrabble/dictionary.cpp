#include "dictionary.h"
#include "exceptions.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>

using namespace std;

string lower(string str) {
  transform(str.cbegin(), str.cend(), str.begin(), ::tolower);
  return str;
}

Dictionary Dictionary::read(const std::string &file_path) {
  ifstream file(file_path);
  if (!file) {
    throw FileException("cannot open dictionary file!");
  }

  std::string word;
  Dictionary dictionary;

  // TODO: complete this. Words in dictionary should all be lowercase.
  // read and add words to the dictionary
  while (!file.fail()) {
    file >> word;
    for (size_t i = 0; i < word.size(); i++) {
      word[i] = tolower(word[i]);
    }
    dictionary.words.insert(word);
  }
  // return the dictionary
  return dictionary;
}

bool Dictionary::is_word(const std::string &word) const {
  return this->words.find(word) != this->words.end();
}
