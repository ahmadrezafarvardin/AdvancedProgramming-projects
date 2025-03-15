#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

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
const string ADD_FLASHCARD_MSG = "flashcards added to the daily box\n";
const string DISPLAY_CARD_MSG =
    "You’ve completed today’s review! Keep the momentum going and continue "
    "building your knowledge, one flashcard at a time!";
const string FLASHCARD = "Flashcard: ";
const string YOUR_ANSWER = "Your answer: ";
const string CORRECT_MSG = "Your answer was correct! Well done, keep it up!";
const string WRONG_MSG =
    "Your answer was incorrect. Don't worry! The correct answer is: ";
const string KEEP_PRACTICE = ". Keep practicing!";
const string STREAK_MSG = "Your current streak is: ";
const string KEEP_GOING = "Keep going!";

#endif