#include "Box.h"
Box::Box(const Box &other) { cards = other.cards; }
void Box::addCard(const FlashCard &card) { cards.push_back(card); }
vector<FlashCard> &Box::getCards() { return cards; }