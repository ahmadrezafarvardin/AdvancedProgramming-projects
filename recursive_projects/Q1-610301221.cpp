#include <iostream>
#include <vector>

const int N_Dimension = 8;
const char EMPTY = '.';
const char QUEEN = 'Q';
const char BLOCK = '*';
typedef std::vector<std::vector<char>> Board;

bool isSafe(Board &board, int row, int col) {
  for (int i = 0; i < col; i++)
    if (board[row][i] == QUEEN)
      return false;

  for (int i = row, j = col; i >= 0 && j >= 0; i--, j--)
    if (board[i][j] == QUEEN)
      return false;

  for (int i = row, j = col; j >= 0 && i < N_Dimension; i++, j--)
    if (board[i][j] == QUEEN)
      return false;

  return true;
}

void solveNQ(Board &board, int col, int &counter) {
  if (col >= N_Dimension) {
    counter++;
    return;
  }

  for (int row = 0; row < N_Dimension; row++) {
    if (board[row][col] != BLOCK && isSafe(board, row, col)) {
      board[row][col] = QUEEN;

      solveNQ(board, col + 1, counter);
      board[row][col] = EMPTY;
    }
  }
}

int countSolutions(Board &board) {
  int counter = 0;
  solveNQ(board, 0, counter);
  return counter;
}

int main() {
  Board board(N_Dimension, std::vector<char>(N_Dimension, EMPTY));

  for (int i = 0; i < N_Dimension; ++i) {
    std::string line;
    std::getline(std::cin, line);
    for (int j = 0; j < N_Dimension; ++j) {
      board[i][j] = line[j];
    }
  }

  int numberOfSolutions = countSolutions(board);
  std::cout << numberOfSolutions << std::endl;

  return 0;
}