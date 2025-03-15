#ifndef APP_H
#define APP_H

#include "Box.h"
#include <tuple>
#include <vector>
using namespace std;

class App {
private:
  vector<Box> boxes;
  int streak = 0;
  int currentDay;
  bool markPracticedToday;
  vector<bool> practiceHistory;
  vector<tuple<int, int, int>> dailyPerformance;

public:
  App();
  void run();
  void getCammand(const string input);
  void addFlashcardToBox(int index, const FlashCard &card);
  void displayStreak();
  void getFlashcards(const string &numberAndCards);
  void displayCards(const string &cardsNum);
  Box collectCards(const int number);
  void nextDay();
  void markPracticed();
  void reviewCard(FlashCard &card, bool correct);
  void addFlashCard(const string &question, const string &answer);
  void getReport(const string &startEndDays);
  void updateDaily(int correct, int incorrect);
  void getProgressReport();
  void nextDayMsg();
  void getReportMsg(int &startDay, int &endDay, int &totalCorrect,
                    int &totalIncorrect);
  void getProgressReportMsg(int &totalParticipationDays,
                            int &masteredFlashcards);

private:
  void showCards(Box &dailyBox);
  bool checkAnswer(const FlashCard &card, const string &userAnswer);
  void moveCard(FlashCard &card, int fromBox, int toBox);
  void handleDays();
};

#endif