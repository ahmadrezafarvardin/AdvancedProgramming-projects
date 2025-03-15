#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

const string STREAK = "streak";
const string ADD_FLASHCARD = "add_flashcard";
const string REVIEW_TODAY = "review_today";
const string GET_REPORT = "get_report";
const string GET_PROGRESS_REPORT = "get_progress_report";
const string NEXT_DAY = "next_day";
const int DAILY = 0;
const int THREE_DAYS = 1;
const int WEEKLY = 2;
const int MONTHLY = 3;
const int SAVED_TO_MEMORY = 4;

int readNumber(const string &input) {
  int number;
  istringstream iss(input);
  iss >> number;
  return number;
}

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

class Box {
private:
  vector<FlashCard> cards;

public:
  void addCard(const FlashCard &card);
  Box() {}
  Box(const Box &other);
  vector<FlashCard> &getCards();
};

Box::Box(const Box &other) { cards = other.cards; }
void Box::addCard(const FlashCard &card) { cards.push_back(card); }
vector<FlashCard> &Box::getCards() { return cards; }
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

private:
  void showCards(Box &dailyBox);
  bool checkAnswer(const FlashCard &card, const string &userAnswer);
  void moveCard(FlashCard &card, int fromBox, int toBox);
  void handleDays();
};
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

void App::nextDay() {
  handleDays();
  currentDay++;
  if (markPracticedToday) {
    streak++;
  } else {
    streak = 0;
  }
  markPracticedToday = false;
  practiceHistory.push_back(false);
  for (auto &box : boxes) {
    for (auto &card : box.getCards()) {
      card.updateReview(false);
    }
  }

  cout << "Good morning! Today is day " << currentDay << " of our journey.\n";
  cout << "Your current streak is: " << streak << "\n";
  cout << "Start reviewing to keep your streak!\n";
}

void App::updateDaily(int correct, int incorrect) {
  if (dailyPerformance.size() < currentDay) {
    dailyPerformance.push_back(make_tuple(currentDay, correct, incorrect));
  } else {
    get<1>(dailyPerformance[currentDay - 1]) += correct;
    get<2>(dailyPerformance[currentDay - 1]) += incorrect;
  }
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
  if (startDay == endDay) {
    cout << "Day: " << startDay << endl;
  } else {
    cout << "Day: " << startDay << " to " << endDay << endl;
  }
  cout << "Correct Answers: " << totalCorrect << endl;
  cout << "Incorrect Answers: " << totalIncorrect << endl;
  cout << "Total: " << (totalCorrect + totalIncorrect) << endl;
}

void App::getProgressReport() {
  int totalParticipationDays =
      count(practiceHistory.begin(), practiceHistory.end(), true);
  int masteredFlashcards = boxes[SAVED_TO_MEMORY].getCards().size();

  cout << "Challenge Progress Report:\n" << endl;
  cout << "Day of the Challenge: " << currentDay << endl;
  cout << "Streak: " << streak << endl;
  cout << "Total Days Participated: " << totalParticipationDays << endl;
  cout << "Mastered Flashcards: " << masteredFlashcards << endl;
  cout << "\nKeep up the great work! You're making steady progress toward "
          "mastering your flashcards."
       << endl;
}

void App::markPracticed() {
  markPracticedToday = true;
  practiceHistory[currentDay - 1] = true;
}

void App::addFlashcardToBox(int boxType, const FlashCard &card) {
  boxes[boxType].addCard(card);
}

void App::displayStreak() {
  cout << "Your current streak is: " << streak << endl;
  cout << "Keep going!" << endl;
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
  cout << "flashcards added to the daily box\n";
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
    cout << "Flashcard: " << card.getQuestion() << endl;

    string userAnswer;
    cout << "Your answer: " << endl;
    getline(cin, userAnswer);

    bool correct = checkAnswer(card, userAnswer);
    reviewCard(card, correct);
    if (correct) {
      cout << "Your answer was correct! Well done, keep it up!" << endl;
    } else {
      cout << "Your answer was incorrect. Don't worry! The correct answer is: "
           << card.getAnswer() << ". Keep practicing!" << endl;
    }
  }
}
void App::displayCards(const string &cardsNum) {
  int number = readNumber(cardsNum);
  Box dailyBox = collectCards(number);
  showCards(dailyBox);
  markPracticed();
  cout
      << R"(You’ve completed today’s review! Keep the momentum going and continue building your knowledge, one flashcard at a time!)"
      << endl;
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

int main() {
  App leitner;
  leitner.run();
  return 0;
}