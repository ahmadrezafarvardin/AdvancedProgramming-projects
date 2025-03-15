#include "DataLoader.hpp"

vector<shared_ptr<Restaurant>> CSVDataLoader::loadRestaurants(
    const string &filename,
    const unordered_map<string, shared_ptr<District>> &districtMap) {
  vector<shared_ptr<Restaurant>> restaurants;
  ifstream file(filename);
  string line;

  getline(file, line);

  while (getline(file, line)) {
    restaurants.push_back(parseRestaurantLine(line, districtMap));
  }

  return restaurants;
}

shared_ptr<Restaurant> CSVDataLoader::parseRestaurantLine(
    const string &line,
    const unordered_map<string, shared_ptr<District>> &districtMap) {
  stringstream ss(line);
  string token;
  vector<string> tokens;

  while (getline(ss, token, TOKEN_SEPERATOR)) {
    tokens.push_back(token);
  }

  string name = tokens[0];
  string districtName = tokens[1];
  auto districtIt = districtMap.find(districtName);
  if (districtIt == districtMap.end()) {
    throw std::runtime_error("District not found: " + districtName);
  }
  shared_ptr<District> district = districtIt->second;
  vector<MenuItem> menu = parseMenu(tokens[2]);
  int openingTime = stoi(tokens[3]);
  int closingTime = stoi(tokens[4]);
  int numTables = stoi(tokens[5]);

  vector<Table> tables;
  for (int i = 0; i < numTables; i++) {
    tables.push_back(Table(i + 1));
  }

  return make_shared<Restaurant>(name, district, menu, openingTime, closingTime,
                                 tables);
}

vector<MenuItem> CSVDataLoader::parseMenu(const string &menuStr) {
  vector<MenuItem> menu;
  stringstream ss(menuStr);
  string foodItem;

  while (getline(ss, foodItem, FOOD_SEPERATOR)) {
    size_t colonPos = foodItem.find(PRICE_IDENTIFIER);
    if (colonPos != string::npos) {
      string foodName = foodItem.substr(0, colonPos);
      double price = stod(foodItem.substr(colonPos + 1));
      menu.push_back(MenuItem(foodName, price));
    }
  }

  return menu;
}

vector<District> CSVDataLoader::loadDistricts(const string &filename) {
  vector<District> districts;
  ifstream file(filename);
  string line;

  getline(file, line);

  while (getline(file, line)) {
    districts.push_back(parseDistrictLine(line));
  }

  return districts;
}

District CSVDataLoader::parseDistrictLine(const std::string &line) {
  istringstream ss(line);
  string districtName, neighborsStr;

  getline(ss, districtName, DISTRICT_SEPARATOR);
  District district(districtName);

  if (getline(ss, neighborsStr)) {
    istringstream neighborsStream(neighborsStr);
    string neighbor;
    while (getline(neighborsStream, neighbor, NEIGHBOR_SEPARATOR)) {
      district.addNeighbor(neighbor);
    }
  }

  return district;
}
