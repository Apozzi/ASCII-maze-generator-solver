#ifndef CHARACTER_MAP
#define CHARACTER_MAP

#include <iostream>
#include <string>
#include <list>

class CharacterMap {
  protected:
    int posx;
    int posy;
    char character;
	int color;

  public:
    CharacterMap(int posx, int posy): posx(posx), posy(posy) {
    }

    void setCharacter(char ch) {
      character = ch;
    }
	
	void setColor(int cor) {
      color = cor;
    }
	
	int getColor() {
      return color;
    }

    char getCharacter() {
      return character;
    }

    int getX() {
      return posx;
    }

    int getY() {
      return posy;
    }
};

#endif
