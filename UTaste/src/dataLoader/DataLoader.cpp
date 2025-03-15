#include "DataLoader.hpp"

vector<shared_ptr<Restaurant>> CSVDataLoader::loadRestaurants(
    const string &restaurantsFile, const string &discountsFile,
    const unordered_map<string, shared_ptr<District>> &districtMap) {

  vector<shared_ptr<Restaurant>> restaurants =
      loadBasicRestaurantsInfo(restaurantsFile, districtMap);
  loadDiscounts(discountsFile, restaurants);
  return restaurants;
}
vector<shared_ptr<Restaurant>> CSVDataLoader::loadBasicRestaurantsInfo(
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

void CSVDataLoader::loadDiscounts(const string &filename,
                                  vector<shared_ptr<Restaurant>> &restaurants) {
  ifstream file(filename);
  if (!file.is_open()) {
    throw runtime_error("Unable to open discount file: " + filename);
  }

  string line;
  getline(file, line);

  while (getline(file, line)) {
    stringstream ss(line);
    string restaurantName, totalDiscount, firstOrderDiscount, foodDiscount;

    getline(ss, restaurantName, TOKEN_SEPERATOR);
    getline(ss, totalDiscount, TOKEN_SEPERATOR);
    getline(ss, firstOrderDiscount, TOKEN_SEPERATOR);
    getline(ss, foodDiscount);

    auto restaurant =
        find_if(restaurants.begin(), restaurants.end(),
                [&restaurantName](const shared_ptr<Restaurant> &r) {
                  return r->getName() == restaurantName;
                });

    if (restaurant == restaurants.end()) {
      throw runtime_error("Restaurant " + restaurantName + "in " + filename +
                          " But Not in <Restaurant File> ");
    }

    if (totalDiscount != NONE) {
      processTotalAmountDiscount(*restaurant, totalDiscount);
    }

    if (firstOrderDiscount != NONE) {
      processFirstOrderDiscount(*restaurant, firstOrderDiscount);
    }

    if (foodDiscount != NONE) {
      processFoodDiscounts(*restaurant, foodDiscount);
    }
  }
}

void CSVDataLoader::processTotalAmountDiscount(
    shared_ptr<Restaurant> &restaurant, const string &discountStr) {
  vector<string> parts = splitString(discountStr, FOOD_SEPERATOR);
  if (parts.size() != 3)
    return;

  bool isPercentage = (parts[0] == PERCENT);
  int minimumAmount = stod(parts[1]);
  int value = stod(parts[2]);

  restaurant->setTotalAmountDiscount(isPercentage, value, minimumAmount);
}

void CSVDataLoader::processFirstOrderDiscount(
    shared_ptr<Restaurant> &restaurant, const string &discountStr) {
  vector<string> parts = splitString(discountStr, FOOD_SEPERATOR);
  if (parts.size() != 2)
    return;

  bool isPercentage = (parts[0] == PERCENT);
  int value = stod(parts[1]);

  restaurant->setFirstOrderDiscount(isPercentage, value);
}

void CSVDataLoader::processFoodDiscounts(shared_ptr<Restaurant> &restaurant,
                                         const string &discountStr) {
  vector<string> discounts = splitString(discountStr, '|');

  for (const auto &discount : discounts) {
    vector<string> parts = splitString(discount, FOOD_SEPERATOR);
    if (parts.size() != 2)
      continue;

    bool isPercentage = (parts[0] == PERCENT);

    vector<string> foodParts = splitString(parts[1], ':');
    if (foodParts.size() != 2)
      continue;

    string foodName = foodParts[0];
    int value = stod(foodParts[1]);

    restaurant->addDishDiscount(foodName, isPercentage, value);
  }
}

vector<string> CSVDataLoader::splitString(const string &str, char delimiter) {
  vector<string> tokens;
  stringstream ss(str);
  string token;

  while (getline(ss, token, delimiter)) {
    tokens.push_back(token);
  }

  return tokens;
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
      int price = stod(foodItem.substr(colonPos + 1));
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
