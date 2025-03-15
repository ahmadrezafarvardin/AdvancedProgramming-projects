#include "FlashCard.h"
#include "constants.h"

FlashCard::FlashCard(string question, string answer)
    : question(question), answer(answer), currentBox(DAILY), wrongAns(0),
      reviewed(false) {}

FlashCard::FlashCard(const FlashCard &other) {
  question = other.question;
  answer = other.answer;
  currentBox = other.currentBox;
  wrongAns = other.wrongAns;
  reviewed = other.reviewed;
}

bool FlashCard::hasReviewed() const { return reviewed; }
void FlashCard::updateReview(bool value) { reviewed = value; }
const int FlashCard::getCurrentBox() const { return currentBox; }
void FlashCard::setCurrentBox(int box) { currentBox = box; }
const int FlashCard::getWrongAns() const { return wrongAns; }
void FlashCard::increaseWrongAns() { ++wrongAns; }
void FlashCard::resetWrongAns() { wrongAns = 0; }
const string &FlashCard::getQuestion() const { return question; }
const string &FlashCard::getAnswer() const { return answer; }
