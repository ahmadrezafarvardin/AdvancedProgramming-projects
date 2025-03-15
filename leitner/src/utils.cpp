#include "utils.h"
#include <sstream>
using namespace std;

int readNumber(const string &input) {
  int number;
  istringstream iss(input);
  iss >> number;
  return number;
}