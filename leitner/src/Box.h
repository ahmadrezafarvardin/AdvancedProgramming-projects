#ifndef BOX_H
#define BOX_H

#include <vector>
#include "FlashCard.h"
#include "constants.h"

class Box {
private:
  vector<FlashCard> cards;

public:
  void addCard(const FlashCard &card);
  Box() {}
  Box(const Box &other);
  vector<FlashCard> &getCards();
};
#endif