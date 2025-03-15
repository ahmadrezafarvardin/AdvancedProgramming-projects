#include <algorithm>
#include <cmath>
#include <iostream>
#include <list>
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
const int SAVED_IN_MEMORY = 4;
const int REVIEW_BOX = 5;
const vector<int> interval = {1, 3, 7, 30};

class FlashCard {
public:
  string question;
  string answer;
  int boxNum;
  bool hasReviewed;
  int wrongAns;
  FlashCard(string question, string answer);
};
FlashCard::FlashCard(string question, string answer)
    : question(question), answer(answer), boxNum(DAILY), hasReviewed(false),
      wrongAns(0) {}

class Box {
public:
  Box(int boxId);
  int boxId;
  list<FlashCard *> cards;

  void addCard(FlashCard *card);
  void removeCard(FlashCard *card);
};

Box::Box(int boxId) : boxId(boxId) {}
void Box::addCard(FlashCard *card) { cards.push_back(card); }
void Box::removeCard(FlashCard *card) { cards.remove(card); }

class App {
public:
  App();
  ~App();
  int streak;
  bool hasStreak;
  vector<Box> boxes;
  int currentDay;
  int correctAns;
  int wrongAns;
  int activeDays;
  void increaseDay();
  void run();
  void getCommand(string &input);
  void printStreak();
  void addNewFlashCards(string &numberAndCards);
  void reviewToday(string &numberOfCards);
  void pickCards(int &number);
  void moveCard(FlashCard *card, Box &fromBox, Box &toBox);
  void reviewCards();
  void handleCard(FlashCard *card, bool ansValue);
  void getReport(const string &startEndDays);
  void getProgressReport();
  void nextDay();
  void handleNextDay();
};

App::App()
    : boxes({Box(DAILY), Box(THREE_DAYS), Box(WEEKLY), Box(MONTHLY),
             Box(SAVED_IN_MEMORY), Box(REVIEW_BOX)}),
      currentDay(1), streak(0), correctAns(0), wrongAns(0), activeDays(0),
      hasStreak(false) {}
App::~App() {
  for (Box &box : boxes) {
    for (FlashCard *card : box.cards) {
      delete card;
    }
  }
}

void App::increaseDay() { currentDay++; }

void App::printStreak() {
  cout << "Your current streak is: " << streak << endl;
  cout << "Keep going!" << endl;
}

void App::addNewFlashCards(string &numberAndCards) {
  istringstream iss(numberAndCards);
  int numberOfCards;
  iss >> numberOfCards;

  string line;
  getline(iss, line);

  for (int i = 0; i < numberOfCards; ++i) {
    string question, answer;
    getline(cin, question);
    getline(cin, answer);
    FlashCard *newCard = new FlashCard(question, answer);
    boxes[DAILY].addCard(newCard);
  }
  cout << "flashcards added to the daily box\n";
}

void App::moveCard(FlashCard *card, Box &fromBox, Box &toBox) {
  card->boxNum = toBox.boxId;
  card->wrongAns = 0;
  toBox.addCard(card);
  fromBox.removeCard(card);
}

void App::pickCards(int &number) {

  int cardCounter = 0;
  for (int type = MONTHLY; type > DAILY; --type) {
    bool shouldPick =
        (!boxes[type].cards.empty()) && (currentDay % interval[type] == 0);
    if (shouldPick && cardCounter < number) {
      auto it = boxes[type].cards.begin();
      while (it != boxes[type].cards.end() && cardCounter < number) {
        FlashCard *movingCard = *it;
        moveCard(movingCard, boxes[type], boxes[REVIEW_BOX]);
        cardCounter++;
        it = boxes[type].cards.erase(it);
      }
    }
  }
  while (cardCounter < number) {
    auto it = boxes[DAILY].cards.begin();
    while (it != boxes[DAILY].cards.end() && cardCounter < number) {
      FlashCard *movingCard = *it;
      moveCard(movingCard, boxes[DAILY], boxes[REVIEW_BOX]);
      cardCounter++;
      it = boxes[DAILY].cards.erase(it);
    }
  }
}

void App::handleCard(FlashCard *card, bool ansValue) {
  if (ansValue) {
    int newId = card->boxNum + 1;
    moveCard(card, boxes[REVIEW_BOX], boxes[newId]);
  } else {
    card->wrongAns++;
    if (card->wrongAns > 1 && !(card->boxNum == DAILY)) {
      int newId = card->boxNum - 1;
      moveCard(card, boxes[REVIEW_BOX], boxes[newId]);
    }
  }
}

void App::reviewCards() {
  for (FlashCard *card : boxes[REVIEW_BOX].cards) {
    cout << "Flashcard: " << card->question << endl;

    string userAnswer;
    cout << "Your answer: " << endl;
    getline(cin, userAnswer);
    bool ansValue = (card->answer == userAnswer);
    card->hasReviewed = true;
    handleCard(card, ansValue);

    if (ansValue) {
      correctAns++;
      cout << "Your answer was correct! Well done, keep it up!" << endl;
    } else {
      wrongAns++;
      cout << "Your answer was incorrect. Don't worry! The correct answer is: "
           << card->answer << ". Keep practicing!" << endl;
    }
  }
}

void App::reviewToday(string &cardNum) {
  istringstream iss(cardNum);
  int number;
  iss >> number;
  hasStreak = true;

  pickCards(number);
  reviewCards();
  activeDays++;
  cout
      << R"(You’ve completed today’s review! Keep the momentum going and continue building your knowledge, one flashcard at a time!)"
      << endl;
}

void App::getReport(const string &startEndDays) {
  istringstream iss(startEndDays);
  int startDay, endDay;
  iss >> startDay >> endDay;

  if (startDay == endDay) {
    cout << "Day: " << startDay << endl;
  } else {
    cout << "Day: " << startDay << " to " << endDay << endl;
  }
  cout << "Correct Answers: " << correctAns << endl;
  cout << "Incorrect Answers: " << wrongAns << endl;
  cout << "Total: " << (correctAns + wrongAns) << endl;
}

void App::getProgressReport() {
  int masteredFlashcards = boxes[SAVED_IN_MEMORY].cards.size();

  cout << "Challenge Progress Report:\n" << endl;
  cout << "Day of the Challenge: " << currentDay << endl;
  cout << "Streak: " << streak << endl;
  cout << "Total Days Participated: " << activeDays << endl;
  cout << "Mastered Flashcards: " << masteredFlashcards << endl;
  cout << "\nKeep up the great work! You're making steady progress toward "
          "mastering your flashcards."
       << endl;
}

void App::handleNextDay() {
  for (int type = THREE_DAYS; type < SAVED_IN_MEMORY; type++) {
    bool shouldPick =
        (!boxes[type].cards.empty()) && (currentDay % interval[type] == 0);
    if (shouldPick) {
      for (FlashCard *card : boxes[type].cards) {
        if (!card->hasReviewed) {
          moveCard(card, boxes[type], boxes[type - 1]);
        }
      }
    }
  }
}

void App::nextDay() {
  if (hasStreak) {
    streak++;
  } else {
    streak = 0;
  }
  handleNextDay();
  currentDay++;
  hasStreak = false;
  for (Box &box : boxes) {
    for (FlashCard *&card : box.cards) {
      card->hasReviewed = false;
    }
  }

  cout << "Good morning! Today is day " << currentDay << " of our journey.\n";
  cout << "Your current streak is: " << streak << "\n";
  cout << "Start reviewing to keep your streak!\n";
}

void App::getCommand(string &input) {
  string command;
  string restOfInput;
  istringstream firstPartOfInput(input);

  firstPartOfInput >> command;
  getline(firstPartOfInput, restOfInput);

  if (command == STREAK) {
    printStreak();
  } else if (command == ADD_FLASHCARD) {
    addNewFlashCards(restOfInput);
  } else if (command == REVIEW_TODAY) {
    reviewToday(restOfInput);
  } else if (command == GET_REPORT) {
    getReport(restOfInput);
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
    getCommand(input);
  }
}

int main() {
  App leitner;
  leitner.run();
  return 0;
}