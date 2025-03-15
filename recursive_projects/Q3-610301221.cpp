#include <climits>
#include <cmath>
#include <iostream>
#include <vector>

int minSweets = INT_MAX;

void calculate(const std::vector<int> &departments, int currentPos,
               int targetPos, int currentSweets) {
  if (currentPos == targetPos) {
    minSweets = std::min(minSweets, currentSweets);
    return;
  }

  if (currentPos > targetPos) {
    return;
  }

  if (currentPos + 1 <= targetPos) {
    int sweetsToGive =
        abs(departments[currentPos + 1] - departments[currentPos]);
    calculate(departments, currentPos + 1, targetPos,
              currentSweets + sweetsToGive);
  }

  if (currentPos + 2 <= targetPos) {
    int sweetsToGive =
        abs(departments[currentPos + 2] - departments[currentPos]);
    calculate(departments, currentPos + 2, targetPos,
              currentSweets + sweetsToGive);
  }
}

int main() {
  int n;
  std::cin >> n;

  std::vector<int> departments(n);
  for (int i = 0; i < n; i++) {
    std::cin >> departments[i];
  }

  calculate(departments, 0, n - 1, 0);

  std::cout << minSweets << std::endl;

  return 0;
}