#ifndef RANDOM_ALPHABET
#define RANDOM_ALPHABET

#include <iostream>
#include <string>
#include <algorithm>
#include <list>
#include <stdlib.h>
#include <time.h>

class RandomAlphabet {
  protected:
    std::list<char> alphabetList;

  public:
  RandomAlphabet() {
    srand (time(NULL));
    loadList();
  }

  bool isLetterOnList(char letter) {
    return (std::find(alphabetList.begin(), alphabetList.end(), letter) != alphabetList.end());
  }

  void loadList() {
    while (alphabetList.size() != 25) {
      int n = rand() % 26;
      char c = (char)(n+65);
      if (!isLetterOnList(c)) {
        alphabetList.push_back(c);
      }
    }
  }

  char getLetter() {
    if (alphabetList.size() != 1) {
      alphabetList.pop_back();
    } else {
      loadList();
    }
    return alphabetList.back();
  }


};

#endif
