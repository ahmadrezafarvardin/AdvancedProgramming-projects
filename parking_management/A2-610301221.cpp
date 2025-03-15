#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

const std::string REQUEST_SPOT = "request_spot";
const std::string ASSIGN_SPOT = "assign_spot";
const std::string CHECKOUT = "checkout";
const std::string PASS_TIME = "pass_time";

const std::string NORMAL = "normal";
const std::string CCTV = "CCTV";
const std::string COVERED = "covered";

const int STATIC_AMOUNT_COVERED = 50;
const int DYNAMIC_AMOUNT_COVERED = 30;
const int STATIC_AMOUNT_CCTV = 80;
const int DYNAMIC_AMOUNT_CCTV = 60;

constexpr std::string_view INITIAL_CONDITION = "empty";
constexpr std::string_view OCCUPIED = "occupied";

struct Expense {
  int pricePerDay;
  int staticPrice;
  int size;

  Expense(int sizeParametr, int staticPriceParametr, int pricePerDayParametr)
      : size(sizeParametr), staticPrice(staticPriceParametr),
        pricePerDay(pricePerDayParametr) {}
};

struct Plot {
  std::string type;
  std::string occupationCondition;
  Expense expenses;
  int size;
  int id;
  int passedDays = 0;

  Plot(int idParametr, int sizeParametr, const std::string &typeParametr,
       Expense expensesParametr)
      : id(idParametr), size(sizeParametr), type(typeParametr),
        expenses(expensesParametr), occupationCondition(INITIAL_CONDITION) {}
};

struct Car {
  std::string name;
  int size;
  std::vector<Plot> matchPlots;

  Car(std::string nameParametr, int sizeParametr)
      : name(nameParametr), size(sizeParametr) {}

  void displayMatchPlots();
};

struct App {
  std::string carsFilePath;
  std::string plotsFilePath;
  std::string expensesFilePath;

  std::vector<Car> cars;
  std::vector<Plot> plots;
  std::vector<Expense> expenses;

  App(const std::string &carsFilePathParam,
      const std::string &plotsFilePathParam,
      const std::string &expensesFilePathParam)
      : carsFilePath(carsFilePathParam), plotsFilePath(plotsFilePathParam),
        expensesFilePath(expensesFilePathParam) {}
  void readInput(std::string input);
  Expense calculateExpenses(const Plot &plot);
  void requestSpot(const std::string &restOfInput);
  void assignSpot(const std::string &idStr);
  void releaseSpot(const int &id);
  void passTime(const std::string &passedDaysStr);
  void checkout(const std::string &idStr);
  int checkoutExpenseCalculator(const Plot &checkedoutPlot,
                                const Expense &initialCheckoutExpense);
  void run();
};

std::vector<Car> readCars(const std::string &carsFilePath) {
  std::ifstream carsFile(carsFilePath);
  std::vector<Car> cars;
  if (!carsFile.is_open()) {
    std::cerr << "Error opening file: " << carsFilePath << std::endl;
    return cars;
  }

  std::string line;
  std::getline(carsFile, line);
  while (std::getline(carsFile, line)) {
    std::stringstream ss(line);
    std::string name;
    std::string sizeStr;

    std::getline(ss, name, ',');
    std::getline(ss, sizeStr, ',');

    int size = std::stoi(sizeStr);

    cars.emplace_back(name, size);
  }

  carsFile.close();
  return cars;
}

std::vector<Plot> readPlots(const std::string &plotsFilePath,
                            const std::vector<Expense> &expenses) {
  std::ifstream plotsFile(plotsFilePath);
  std::vector<Plot> plots;
  if (!plotsFile.is_open()) {
    std::cerr << "Error opening file: " << plotsFilePath << std::endl;
    return plots;
  }
  std::string line;
  std::getline(plotsFile, line);
  while (std::getline(plotsFile, line)) {
    std::stringstream ss(line);
    std::string idStr, sizeStr, type;

    std::getline(ss, idStr, ',');
    std::getline(ss, sizeStr, ',');
    std::getline(ss, type);

    int id = std::stoi(idStr);
    int size = std::stoi(sizeStr);

    auto it = std::find_if(
        expenses.begin(), expenses.end(),
        [size](const Expense &expense) { return expense.size == size; });
    Expense plotExpense(size, 0, 0);
    if (it != expenses.end()) {
      plotExpense = *it;
    } else {
      std::cerr << "No matching expense found for size: " << size << std::endl;
    }

    plots.emplace_back(id, size, type, plotExpense);
  }

  plotsFile.close();
  return plots;
}

std::vector<Expense> readExpenses(const std::string &expensesFilePath) {
  std::ifstream expensesFile(expensesFilePath);
  std::vector<Expense> expenses;

  if (!expensesFile.is_open()) {
    std::cerr << "Error opening file: " << expensesFilePath << std::endl;
    return expenses;
  }

  std::string line;
  std::getline(expensesFile, line);
  while (std::getline(expensesFile, line)) {
    std::stringstream ss(line);
    std::string staticPriceStr;
    std::string sizeStr;
    std::string pricePerDayStr;

    std::getline(ss, sizeStr, ',');
    std::getline(ss, staticPriceStr, ',');
    std::getline(ss, pricePerDayStr);

    int size = std::stoi(sizeStr);
    int staticPrice = std::stoi(staticPriceStr);
    int pricePerDay = std::stoi(pricePerDayStr);
    expenses.emplace_back(size, staticPrice, pricePerDay);
  }

  expensesFile.close();
  return expenses;
}

void Car::displayMatchPlots() {
  std::sort(matchPlots.begin(), matchPlots.end(),
            [](const Plot &a, const Plot &b) { return a.id < b.id; });
  for (auto &plot : matchPlots) {
    std::cout << plot.id << ": " << plot.type << " "
              << plot.expenses.staticPrice << " " << plot.expenses.pricePerDay
              << "\n";
  }
}

Expense getBasePrices(const std::vector<Expense> &expenses, int size) {
  for (const auto &expense : expenses) {
    if (expense.size == size) {
      return expense;
    }
  }
  if (expenses.empty()) {
    throw std::runtime_error("Expenses list is empty.");
  }
  return Expense(size, 0, 0);
}

void adjustPricesByType(const std::string &plotType, int &staticPrice,
                        int &pricePerDay) {
  if (plotType == COVERED) {
    staticPrice = static_cast<int>(staticPrice + STATIC_AMOUNT_COVERED);
    pricePerDay = static_cast<int>(pricePerDay + DYNAMIC_AMOUNT_COVERED);
  } else if (plotType == CCTV) {
    staticPrice = static_cast<int>(staticPrice + STATIC_AMOUNT_CCTV);
    pricePerDay = static_cast<int>(pricePerDay + DYNAMIC_AMOUNT_CCTV);
  } else {
    std::cerr << "Unknown plot type: " << plotType << std::endl;
  }
}

Expense App::calculateExpenses(const Plot &plot) {
  Expense baseExpense = getBasePrices(expenses, plot.size);

  int staticPrice = baseExpense.staticPrice;
  int pricePerDay = baseExpense.pricePerDay;

  if (plot.type != NORMAL) {
    adjustPricesByType(plot.type, staticPrice, pricePerDay);
  }

  return Expense(plot.size, staticPrice, pricePerDay);
}

void App::passTime(const std::string &passedDaysStr) {
  int passedDays = stoi(passedDaysStr);
  for (auto &plot : plots) {
    if (plot.occupationCondition == OCCUPIED) {
      plot.passedDays += passedDays;
    }
  }
}

int App::checkoutExpenseCalculator(const Plot &checkedoutPlot,
                                   const Expense &initialCheckoutExpense) {
  int sum = initialCheckoutExpense.staticPrice +
            checkedoutPlot.passedDays * initialCheckoutExpense.pricePerDay;
  return sum;
}

void App::checkout(const std::string &idStr) {
  int id = stoi(idStr);
  auto plotIt = std::find_if(plots.begin(), plots.end(),
                             [&id](const Plot &plot) { return plot.id == id; });

  if (plotIt != plots.end()) {
    Plot &plot = *plotIt;

    for (const auto &car : cars) {
      if (car.size == plot.size && plot.occupationCondition == OCCUPIED) {
        Expense initialCheckoutExpense = calculateExpenses(plot);
        Plot checkedoutPlot = plot;
        int finalCheckoutExpense =
            checkoutExpenseCalculator(checkedoutPlot, initialCheckoutExpense);
        releaseSpot(checkedoutPlot.id);
        std::cout << "Spot " << checkedoutPlot.id << " is free now.\n";
        std::cout << "Total cost: " << finalCheckoutExpense << std::endl;
      }
    }
  } else {
    std::cout << "Spot with id: " << plotIt->id << " not found to release\n";
  }
}

void App::assignSpot(const std::string &idStr) {
  int id = std::stoi(idStr);

  auto it = std::find_if(plots.begin(), plots.end(),
                         [id](const Plot &plot) { return plot.id == id; });

  if (it != plots.end()) {
    it->occupationCondition = OCCUPIED;
    std::cout << "Spot " << id << " is occupied now." << std::endl;
  } else {
    std::cout << "Spot ID not found." << std::endl;
  }
}

void App::releaseSpot(const int &id) {

  auto it = std::find_if(plots.begin(), plots.end(),
                         [id](const Plot &plot) { return plot.id == id; });

  if (it != plots.end()) {
    it->occupationCondition = INITIAL_CONDITION;
    it->passedDays = 0;
  } else {
    std::cout << "Spot ID not found for checkout." << std::endl;
  }
}

void App::requestSpot(const std::string &nameOfCar) {
  auto carIt =
      std::find_if(cars.begin(), cars.end(), [&nameOfCar](const Car &car) {
        return car.name == nameOfCar;
      });

  if (carIt != cars.end()) {
    Car &car = *carIt;
    car.matchPlots.clear();

    for (const auto &plot : plots) {
      if (car.size == plot.size &&
          plot.occupationCondition == INITIAL_CONDITION) {
        Expense calculatedExpense = calculateExpenses(plot);
        Plot modifiedPlot = plot;
        modifiedPlot.expenses = calculatedExpense;
        car.matchPlots.push_back(modifiedPlot);
      }
    }

    car.displayMatchPlots();
  } else {
    std::cout << "Car not found: " << nameOfCar << "\n";
  }
}

void App::readInput(const std::string input) {
  std::string command;
  std::string restOfInput;
  std::istringstream firstParsedInput(input);

  firstParsedInput >> command;
  std::getline(firstParsedInput, restOfInput);
  restOfInput.erase(0, restOfInput.find_first_not_of(' '));

  if (command == REQUEST_SPOT) {
    requestSpot(restOfInput);
  } else if (command == ASSIGN_SPOT) {
    assignSpot(restOfInput);
  } else if (command == PASS_TIME) {
    passTime(restOfInput);
  } else if (command == CHECKOUT) {
    checkout(restOfInput);
  } else {
    std::cout << "Unknown command: " << command << "\n";
  }
}

void App::run() {
  cars = readCars(carsFilePath);
  expenses = readExpenses(expensesFilePath);
  plots = readPlots(plotsFilePath, expenses);

  std::string input;

  while (std::getline(std::cin, input)) {
    if (input.empty()) {
      continue;
    }
    readInput(input);
  }
}

int main(int argc, char *argv[]) {
  if (argc < 4)
    return 1;
  std::string carsFilePath;
  std::string plotsFilePath;
  std::string expensesFilePath;
  carsFilePath = argv[1];
  plotsFilePath = argv[2];
  expensesFilePath = argv[3];
  App parkingManagement(carsFilePath, plotsFilePath, expensesFilePath);
  parkingManagement.run();
  return 0;
}