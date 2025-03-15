#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

constexpr std::string_view INITIAL_CONDITION = "empty";

struct Car {
  std::string name;
  int size;

  Car(std::string nameParametr, int sizeParametr)
      : name(nameParametr), size(sizeParametr) {}
};

struct ParkingPlot {
  std::string type;
  std::string occupationCondition;
  int size;
  int id;

  ParkingPlot(int idParametr, int sizeParametr, const std::string &typeParametr)
      : id(idParametr), size(sizeParametr), type(typeParametr),
        occupationCondition(INITIAL_CONDITION) {}
};

struct Expense
{
  int pricePerDay;
  int staticPrice;
  int size;

  Expense(int size, int staticPrice, int prcePerDay);
};


std::vector<Car> readCars(const std::string carsFilePath) {
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

std::vector<ParkingPlot>
readParkingPlots(const std::string &parkingPlotsFilePath) {
  std::ifstream parkingPlotsFile(parkingPlotsFilePath);
  std::vector<ParkingPlot> parkingPlots;
  if (!parkingPlotsFile.is_open()) {
    std::cerr << "Error opening file: " << parkingPlotsFilePath << std::endl;
    return parkingPlots;
  }

  std::string line;
  std::getline(parkingPlotsFile, line);
  while (std::getline(parkingPlotsFile, line)) {
    std::stringstream ss(line);
    std::string idStr;
    std::string sizeStr;
    std::string type;

    std::getline(ss, idStr, ',');
    std::getline(ss, sizeStr, ',');
    std::getline(ss, type);

    int size = std::stoi(sizeStr);
    int id = std::stoi(idStr);

    parkingPlots.emplace_back(id, size, type);
  }

  parkingPlotsFile.close();
  return parkingPlots;
}

void run(const std ::string &carsFilePath,
         const std::string &parkingPlotsFilePath,
         std::string &expensesFilePath) {
  std::vector<Car> cars = readCars(carsFilePath);
  std::vector<ParkingPlot> parkingPlots =
      readParkingPlots(parkingPlotsFilePath);
}

int main(int argc, char *argv[]) {
  if (argc < 4)
    return 1;
  std::string carsFilePath;
  std::string parkingPlotsFilePath;
  std::string expensesFilePath;
  carsFilePath = argv[1];
  parkingPlotsFilePath = argv[2];
  expensesFilePath = argv[3];
  run(carsFilePath, parkingPlotsFilePath, expensesFilePath);
  return 0;
}
