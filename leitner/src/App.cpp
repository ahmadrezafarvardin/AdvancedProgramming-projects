#include "App.h"
#include "constants.h"
#include "utils.h"
#include <algorithm>
#include <iostream>
#include <sstream>

App::App() : boxes(5), streak(0), currentDay(1), markPracticedToday(false) {
  practiceHistory.push_back(false);
}

void App::moveCard(FlashCard &card, int fromBox, int toBox) {
  auto &fromBoxCards = boxes[fromBox].getCards();

  for (auto it = fromBoxCards.begin(); it != fromBoxCards.end(); ++it) {
    if (it->getQuestion() == card.getQuestion()) {
      FlashCard &updatedCard = *it;
      updatedCard.resetWrongAns();
      updatedCard.setCurrentBox(toBox);
      updatedCard.updateReview(true);
      boxes[toBox].addCard(updatedCard);
      fromBoxCards.erase(it);
      card = updatedCard;

      return;
    }
  }
}

void App::reviewCard(FlashCard &card, bool correct) {
  int currentBox = card.getCurrentBox();
  updateDaily(correct ? 1 : 0, correct ? 0 : 1);
  card.updateReview(true);

  if (correct) {
    int newBox = currentBox + 1;
    moveCard(card, currentBox, newBox);
  } else {
    card.increaseWrongAns();
    if (currentBox != DAILY && card.getWrongAns() >= 2) {
      int newBox = currentBox - 1;
      moveCard(card, currentBox, newBox);
    }
  }
}

void App::addFlashCard(const string &question, const string &answer) {
  FlashCard newCard(question, answer);
  boxes[DAILY].addCard(newCard);
}

void App::handleDays() {
  vector<int> intervals = {1, 3, 7, 30};

  for (int index = THREE_DAYS; index < SAVED_TO_MEMORY; ++index) {
    auto &boxCards = boxes[index].getCards();

    for (auto it = boxCards.begin(); it != boxCards.end();) {
      if (currentDay % intervals[index] == 0 && !(it->hasReviewed())) {

        FlashCard card = *it;
        card.setCurrentBox(index - 1);
        boxes[index - 1].addCard(card);
        it = boxCards.erase(it);
      } else {
        ++it;
      }
    }
  }
}

void App::nextDayMsg() {
  cout << "Good morning! Today is day " << currentDay << " of our journey.\n";
  cout << "Your current streak is: " << streak << "\n";
  cout << "Start reviewing to keep your streak!\n";
}

void App::nextDay() {
  handleDays();
  currentDay++;
  if (!markPracticedToday) {
    streak = 0;
  }
  markPracticedToday = false;
  practiceHistory.push_back(false);
  for (auto &box : boxes) {
    for (auto &card : box.getCards()) {
      card.updateReview(false);
    }
  }
  nextDayMsg();
}

void App::updateDaily(int correct, int incorrect) {
  if (static_cast<int>(dailyPerformance.size()) < currentDay) {
    dailyPerformance.push_back(make_tuple(currentDay, correct, incorrect));
  } else {
    get<1>(dailyPerformance[currentDay - 1]) += correct;
    get<2>(dailyPerformance[currentDay - 1]) += incorrect;
  }
}

void App::getReportMsg(int &startDay, int &endDay, int &totalCorrect,
                       int &totalIncorrect) {
  if (startDay == endDay) {
    cout << "Day: " << startDay << endl;
  } else {
    cout << "Day: " << startDay << " to " << endDay << endl;
  }
  cout << "Correct Answers: " << totalCorrect << endl;
  cout << "Incorrect Answers: " << totalIncorrect << endl;
  cout << "Total: " << (totalCorrect + totalIncorrect) << endl;
}

void App::getReport(const string &startEndDays) {
  istringstream iss(startEndDays);
  int startDay, endDay;
  iss >> startDay >> endDay;

  int totalCorrect = 0, totalIncorrect = 0;

  for (const auto &dayPerformance : dailyPerformance) {
    int day = get<0>(dayPerformance);
    if (day >= startDay && day <= endDay) {
      totalCorrect += get<1>(dayPerformance);
      totalIncorrect += get<2>(dayPerformance);
    }
  }

  getReportMsg(startDay, endDay, totalCorrect, totalIncorrect);
}

void App::getProgressReportMsg(int &totalParticipationDays,
                               int &masteredFlashcards) {
  cout << "Challenge Progress Report:\n" << endl;
  cout << "Day of the Challenge: " << currentDay << endl;
  cout << "Streak: " << streak << endl;
  cout << "Total Days Participated: " << totalParticipationDays << endl;
  cout << "Mastered Flashcards: " << masteredFlashcards << endl;
  cout << "\nKeep up the great work! You're making steady progress toward "
          "mastering your flashcards."
       << endl;
}

void App::getProgressReport() {
  int totalParticipationDays =
      count(practiceHistory.begin(), practiceHistory.end(), true);
  int masteredFlashcards = boxes[SAVED_TO_MEMORY].getCards().size();
  getProgressReportMsg(totalParticipationDays, masteredFlashcards);
}

void App::markPracticed() {
  markPracticedToday = true;
  practiceHistory[currentDay - 1] = true;
}

void App::addFlashcardToBox(int boxType, const FlashCard &card) {
  boxes[boxType].addCard(card);
}

void App::displayStreak() {
  cout << STREAK_MSG << streak << endl;
  cout << KEEP_GOING << endl;
}

void App::getFlashcards(const string &numberAndCards) {
  istringstream iss(numberAndCards);
  int cardsNum;
  iss >> cardsNum;

  string line;
  getline(iss, line);

  for (int i = 0; i < cardsNum; ++i) {
    string question, answer;
    getline(cin, question);
    getline(cin, answer);
    addFlashCard(question, answer);
  }
  cout << ADD_FLASHCARD_MSG;
}

Box App::collectCards(const int number) {
  Box dailyBox;
  int cardsCollected = 0;

  if (currentDay % 30 == 0) {
    for (auto &card : boxes[MONTHLY].getCards()) {
      if (cardsCollected >= number)
        break;
      if (!card.hasReviewed()) {
        dailyBox.addCard(card);
        cardsCollected++;
      }
    }
  }

  if (currentDay % 7 == 0) {
    for (auto &card : boxes[WEEKLY].getCards()) {
      if (cardsCollected >= number)
        break;
      if (!card.hasReviewed()) {
        dailyBox.addCard(card);
        cardsCollected++;
      }
    }
  }

  if (currentDay % 3 == 0) {
    for (auto &card : boxes[THREE_DAYS].getCards()) {
      if (cardsCollected >= number)
        break;
      if (!card.hasReviewed()) {
        dailyBox.addCard(card);
        cardsCollected++;
      }
    }
  }

  for (auto &card : boxes[DAILY].getCards()) {
    if (cardsCollected >= number)
      break;
    if (!card.hasReviewed()) {
      dailyBox.addCard(card);
      cardsCollected++;
    }
  }

  return dailyBox;
}

void App::showCards(Box &dailyBox) {
  auto &cards = dailyBox.getCards();
  for (auto &card : cards) {
    cout << FLASHCARD << card.getQuestion() << endl;

    string userAnswer;
    cout << YOUR_ANSWER << endl;
    getline(cin, userAnswer);

    bool correct = checkAnswer(card, userAnswer);
    reviewCard(card, correct);
    if (correct) {
      cout << CORRECT_MSG << endl;
    } else {
      cout << WRONG_MSG << card.getAnswer() << KEEP_PRACTICE << endl;
    }
  }
}
void App::displayCards(const string &cardsNum) {
  int number = readNumber(cardsNum);
  Box dailyBox = collectCards(number);
  showCards(dailyBox);
  if (!markPracticedToday) {
    streak++;
  };
  markPracticed();
  cout << DISPLAY_CARD_MSG << endl;
}

bool App::checkAnswer(const FlashCard &card, const string &userAnswer) {
  return card.getAnswer() == userAnswer;
}

void App::getCammand(const string input) {
  string command;
  string rest;
  istringstream firstPart(input);

  firstPart >> command;
  getline(firstPart, rest);

  if (command == STREAK) {
    displayStreak();
  } else if (command == ADD_FLASHCARD) {
    getFlashcards(rest);
  } else if (command == REVIEW_TODAY) {
    displayCards(rest);
  } else if (command == GET_REPORT) {
    getReport(rest);
  } else if (command == GET_PROGRESS_REPORT) {
    getProgressReport();
  } else if (command == NEXT_DAY) {
    nextDay();
  }
}

void App::run() {
  string input;
  while (getline(cin, input)) {
    if (input.empty()) {
      continue;
    }
    getCammand(input);
  }
}
