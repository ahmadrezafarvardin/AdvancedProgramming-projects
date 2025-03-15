#include <iostream>
#include <set>
#include <string>
#include <vector>

int evaluate(int left, int right, char op) {
  switch (op) {
  case '+':
    return left + right;
  case '-':
    return left - right;
  case '*':
    return left * right;
  default:
    throw std::invalid_argument("Invalid operator");
  }
}

std::set<int> resultComputer(const std::string &expression) {
  std::set<int> results;

  if (expression.size() == 1) {
    results.insert(expression[0] - '0');
    return results;
  }

  for (size_t i = 1; i < expression.size(); i += 2) {
    char op = expression[i];

    std::string leftPart = expression.substr(0, i);
    std::string rightPart = expression.substr(i + 1);

    std::set<int> leftResults = resultComputer(leftPart);
    std::set<int> rightResults = resultComputer(rightPart);

    for (int leftValue : leftResults) {
      for (int rightValue : rightResults) {
        results.insert(evaluate(leftValue, rightValue, op));
      }
    }
  }

  return results;
}

int main() {
  int n;
  std::string input;
  std::cin >> n;
  std::cin >> input;

  std::set<int> distincResults = resultComputer(input);
  std::cout << distincResults.size() << std::endl;

  return 0;
}