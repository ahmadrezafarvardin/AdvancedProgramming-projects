#ifndef FLASHCARD_H
#define FLASHCARD_H

#include<string>
using namespace std;

class FlashCard {
private:
  string question;
  string answer;
  int currentBox;
  int wrongAns;
  bool reviewed;

public:
  FlashCard(string question, string answer);
  FlashCard(const FlashCard &other);
  const string &getQuestion() const;
  const string &getAnswer() const;
  void increaseWrongAns();
  void resetWrongAns();
  const int getCurrentBox() const;
  const int getWrongAns() const;
  void setCurrentBox(int box);
  int getCurrentDay() const;
  void setCurrentDay(int day);
  bool hasReviewed() const;
  void updateReview(bool value);
};
#endif