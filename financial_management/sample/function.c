#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

constexpr std::string_view INITIAL_CONDITION = "empty";

struct ParkingPlot {
  std::string type;
  std::string occupationCondition;
  int size;
  int id;

  ParkingPlot(int idParametr, int sizeParametr, const std::string &typeParametr)
      : id(idParametr), size(sizeParametr), type(typeParametr),
        occupationCondition(INITIAL_CONDITION) {}
};

std::vector<ParkingPlot>
readParkingPlots(const std::string &parkingPlotsFilePath) {
  std::ifstream parkingPlotsFile(parkingPlotsFilePath);
  std::vector<ParkingPlot> parkingPlots;

  if (!parkingPlotsFile.is_open()) {
    std::cerr << "Error opening file: " << parkingPlotsFilePath << std::endl;
    return parkingPlots;
  }

  std::string line;
  std::getline(parkingPlotsFile, line); // Read header line
  while (std::getline(parkingPlotsFile, line)) {
    std::stringstream ss(line);
    std::string idStr;
    std::string sizeStr;
    std::string type;

    std::getline(ss, idStr, ',');
    std::getline(ss, sizeStr, ',');
    std::getline(ss, type);

    try {
      int size = std::stoi(sizeStr);
      int id = std::stoi(idStr);
      parkingPlots.emplace_back(id, size, type);
    } catch (const std::invalid_argument &e) {
      std::cerr << "Invalid input in file: " << line << std::endl;
    } catch (const std::out_of_range &e) {
      std::cerr << "Input value out of range in file: " << line << std::endl;
    }
  }

  parkingPlotsFile.close();
  return parkingPlots;
}

void run(const std::string &parkingPlotsFilePath) {
  std::vector<ParkingPlot> parkingPlots =
      readParkingPlots(parkingPlotsFilePath);

  // Output the parking plots
  std::cout << "Parking Plots:\n";
  for (const auto &plot : parkingPlots) {
    std::cout << "ID: " << plot.id << ", Size: " << plot.size
              << ", Type: " << plot.type
              << ", Occupation Condition: " << plot.occupationCondition << "\n";
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <parking_plots_file>" << std::endl;
    return 1;
  }

  std::string parkingPlotsFilePath = argv[1];
  run(parkingPlotsFilePath); // Run the function with provided file path
  return 0;
}
