#include "utilities.hpp"
#include "../systemCore/sysCoreInclude/District.hpp"
#include "../systemCore/sysCoreInclude/MenuItem.hpp"
#include "../systemCore/sysCoreInclude/Restaurant.hpp"
#include "../systemCore/sysCoreInclude/Table.hpp"

string formatSingleDistrict(const District &district) {
  string result = district.getName() + ": ";
  const auto &neighbors = district.getNeighbors();
  for (size_t i = 0; i < neighbors.size(); ++i) {
    result += neighbors[i];
    if (i < neighbors.size() - 1) {
      result += ", ";
    }
  }
  return result;
}

string formatDistrictInfo(const vector<District> &districts) {
  string result;
  for (size_t i = 0; i < districts.size(); ++i) {
    result += formatSingleDistrict(districts[i]);
    if (i < districts.size() - 1) {
      result += "\n";
    }
  }
  return result;
}

string formatRestList(const vector<pair<string, string>> &restaurants) {
  string formattedRests;
  for (size_t i = 0; i < restaurants.size(); ++i) {
    formattedRests += restaurants[i].first + " (" + restaurants[i].second + ")";
    if (i < restaurants.size() - 1) {
      formattedRests += "\n";
    }
  }
  return formattedRests;
}

string formatMenuItems(const vector<MenuItem> &menu) {
  vector<string> menuItems;
  for (const auto &item : menu) {
    menuItems.push_back(item.getName() + "(" + to_string(item.getPrice()) +
                        ")");
  }
  sort(menuItems.begin(), menuItems.end());

  string result;
  for (size_t i = 0; i < menuItems.size(); i++) {
    result += menuItems[i];
    if (i < menuItems.size() - 1) {
      result += ", ";
    }
  }
  return result;
}

string formatTableResis(const vector<Table> &tables) {
  string result;
  for (size_t i = 0; i < tables.size(); i++) {
    result += to_string(tables[i].getId()) + ":";

    const auto &reservations = tables[i].getReservations();
    if (!reservations.empty()) {
      result += " ";
      for (size_t i = 0; i < reservations.size(); i++) {
        result += "(" + to_string(reservations[i].getStartTime()) + "-" +
                  to_string(reservations[i].getEndTime()) + ")";
        if (i < reservations.size() - 1) {
          result += ", ";
        }
      }
    }
    if (i < tables.size() - 1) {
      result += "\n";
    }
  }
  return result;
}

string formatRestDetails(const shared_ptr<Restaurant> &restaurant) {
  string response;

  response += "Name: " + restaurant->getName() + "\n";

  response += "District: " + restaurant->getDistrict()->getName() + "\n";

  response += "Time: " + to_string(restaurant->getOpeningTime()) + "-" +
              to_string(restaurant->getClosingTime()) + "\n";

  response += "Menu: " + formatMenuItems(restaurant->getMenu()) + "\n";

  response += formatTableResis(restaurant->getTables());

  return response;
}

vector<string> parseFoodList(const string &foodString) {
  vector<string> foods;
  string currentFood;
  bool hasComma = foodString.find(',') != string::npos;

  if (hasComma) {
    istringstream ss(foodString);
    while (getline(ss, currentFood, ',')) {
      if (!currentFood.empty()) {
        foods.push_back(currentFood);
      }
    }
  } else {
    istringstream ss(foodString);
    string word;
    while (ss >> word) {
      currentFood += (currentFood.empty() ? "" : " ") + word;
    }
    if (!currentFood.empty()) {
      foods.push_back(currentFood);
    }
  }

  return foods;
}

pair<unordered_map<string, string>, vector<string>>
parseParameters(const vector<string> &params) {
  unordered_map<string, string> reserveParams;
  vector<string> foods;

  for (size_t i = 0; i < params.size(); i += 2) {
    if (i + 1 >= params.size())
      break;

    string key = params[i];
    string value = params[i + 1];

    if (key == "foods") {
      foods = parseFoodList(value);
    } else {
      reserveParams[key] = value;
    }
  }

  return {reserveParams, foods};
}

pair<bool, tuple<string, int, int, int>>
extractReservationParams(const unordered_map<string, string> &reserveParams) {
  try {
    string restaurantName = reserveParams.at("restaurant_name");
    int tableId = stoi(reserveParams.at("table_id"));
    int startTime = stoi(reserveParams.at("start_time"));
    int endTime = stoi(reserveParams.at("end_time"));

    return {true, {restaurantName, tableId, startTime, endTime}};
  } catch (const exception &e) {
    return {false, {"", 0, 0, 0}};
  }
}

bool isValidTimeRange(int startTime, int endTime) {
  return startTime >= 1 && endTime <= 24 && startTime < endTime;
}

bool isWithinRestaurantHours(const shared_ptr<Restaurant> &restaurant,
                             int startTime, int endTime) {
  return startTime >= restaurant->getOpeningTime() &&
         endTime <= restaurant->getClosingTime();
}

pair<bool, int>
validateAndCalculateOrder(const shared_ptr<Restaurant> &restaurant,
                          const vector<string> &foods) {
  int totalPrice = 0;
  for (const auto &foodName : foods) {
    const MenuItem *menuItem = restaurant->findMenuItem(foodName);
    if (!menuItem) {
      return {false, 0};
    }
    totalPrice += menuItem->getPrice();
  }
  return {true, totalPrice};
}

shared_ptr<Table> findTable(const shared_ptr<Restaurant> &restaurant,
                            int tableId) {
  auto table = restaurant->findTable(tableId);
  return table ? make_shared<Table>(*table) : nullptr;
}

string formatReservationResponse(int reservationId,
                                 const string &restaurantName, int tableId,
                                 int startTime, int endTime,
                                 [[maybe_unused]] const vector<string> &foods,
                                 int totalPrice) {
  string response = "Reserve ID: " + to_string(reservationId) + "\n";
  response += "Table " + to_string(tableId) + " for " + to_string(startTime) +
              " to " + to_string(endTime) + " in " + restaurantName + "\n";
  response += "Price: " + to_string(totalPrice);
  return response;
}

string formatReservationDetails(const Reservation &reservation) {
  string result = to_string(reservation.getId()) + ": ";
  result += reservation.getRestaurantName() + " ";
  result += to_string(reservation.getTableId()) + " ";

  result += to_string(reservation.getStartTime()) + "-" +
            to_string(reservation.getEndTime()) + " ";

  const auto &foods = reservation.getOrderedFoods();
  if (!foods.empty()) {
    for (size_t i = 0; i < foods.size(); ++i) {
      result += foods[i].first + "(" + to_string(foods[i].second) + ")";
      if (i < foods.size() - 1)
        result += " ";
    }
  }
  return result;
}