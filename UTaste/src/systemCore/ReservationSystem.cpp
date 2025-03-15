#include "sysCoreInclude/ReservationSystem.hpp"

void ReservationSystem::initialize(const string &restaurantsFile,
                                   const string &districtsFile,
                                   const string &discountFile) {
  try {
    districts_ = dataLoader_->loadDistricts(districtsFile);
    districtMap_.clear();

    for (const auto &district : districts_) {
      auto districtPtr = make_shared<District>(district);
      districtMap_[districtPtr->getName()] = districtPtr;
    }

    restaurants_ = dataLoader_->loadRestaurants(restaurantsFile, discountFile,
                                                districtMap_);
  } catch (const exception &e) {
    cerr << "Error during initialization: " << e.what() << endl;
  }
}

Response ReservationSystem::signup(const string &username,
                                   const string &password) {
  if (username.empty() || password.empty()) {
    return BadRequestResponse();
  }
  if (usernames_.find(username) != usernames_.end()) {
    return BadRequestResponse();
  }
  if (currentUser_ != nullptr && currentUser_->getName() == username) {
    return PermissionDeniedResponse();
  }

  users_.emplace_back(User(username, password));
  usernames_.insert(username);
  // login(username, password);
  return OkResponse();
}

Response ReservationSystem::login(const string &username,
                                  const string &password) {
  if (isLoggedIn()) {
    return PermissionDeniedResponse();
  }
  if (usernames_.find(username) == usernames_.end()) {
    return NotFoundResponse();
  }

  auto userIt = find_if(users_.begin(), users_.end(), [&username](User &user) {
    return user.getName() == username;
  });

  if (userIt->getPsw() != password) {
    return PermissionDeniedResponse();
  }

  if (isLoggedIn() && currentUser_->getName() == username) {
    return PermissionDeniedResponse();
  }

  currentUser_ = &(*userIt);

  return OkResponse();
}

Response ReservationSystem::logout() {
  if (!isLoggedIn()) {
    return PermissionDeniedResponse();
  }
  currentUser_ = nullptr;
  return OkResponse();
}

Response ReservationSystem::getDistricts(const string &districtName) {
  if (!isLoggedIn()) {
    return PermissionDeniedResponse();
  }
  string formattedDists;
  if (districtName.empty()) {
    if (districts_.empty()) {
      return EmptyResponse();
    }
    sort(districts_.begin(), districts_.end(),
         [](const District &a, const District &b) {
           return a.getName() < b.getName();
         });
    formattedDists = formatDistrictInfo(districts_);
  } else {
    auto districtIt = districtMap_.find(districtName);
    if (districtIt == districtMap_.end()) {
      return NotFoundResponse();
    }
    shared_ptr<District> district = districtIt->second;
    formattedDists = formatSingleDistrict(*district);
  }
  return Response(formattedDists);
}

Response ReservationSystem::setUserDistrict(const string &districtName) {
  if (!isLoggedIn()) {
    return PermissionDeniedResponse();
  }
  auto districtIt = districtMap_.find(districtName);
  if (districtIt == districtMap_.end()) {
    return NotFoundResponse();
  }

  shared_ptr<District> district = districtIt->second;
  currentUser_->setLocation(district);

  return OkResponse();
}

Response ReservationSystem::getRestaurants(const string &foodName) {
  if (!isLoggedIn()) {
    return PermissionDeniedResponse();
  }

  if (currentUser_->getCurrentDistrict() == nullptr) {
    return NotFoundResponse();
  }

  string currentDistrictName = currentUser_->getCurrentDistrict()->getName();
  vector<string> orderedDistricts = getBFSDists(currentDistrictName);
  vector<pair<string, string>> filteredRestaurants =
      filterSortRests(orderedDistricts, foodName);
  if (filteredRestaurants.empty()) {
    return EmptyResponse();
  }
  string formattedRests = formatRestList(filteredRestaurants);

  return Response(formattedRests);
}

vector<string>
ReservationSystem::getBFSDists(const string &startDistrict) const {
  queue<string> districtQueue;
  unordered_set<string> visitedDistricts;
  vector<string> orderedDistricts;

  districtQueue.push(startDistrict);
  visitedDistricts.insert(startDistrict);

  while (!districtQueue.empty()) {
    string distName = districtQueue.front();
    districtQueue.pop();
    orderedDistricts.push_back(distName);

    auto distIt = districtMap_.find(distName);
    if (distIt != districtMap_.end()) {
      for (const auto &neighborName : distIt->second->getNeighbors()) {
        if (visitedDistricts.find(neighborName) == visitedDistricts.end()) {
          districtQueue.push(neighborName);
          visitedDistricts.insert(neighborName);
        }
      }
    }
  }

  return orderedDistricts;
}

vector<pair<string, string>>
ReservationSystem::filterSortRests(const vector<string> &orderedDistricts,
                                   const string &foodName) const {
  vector<pair<string, string>> filteredRestaurants;

  for (const auto &distName : orderedDistricts) {
    vector<string> districtRestaurants;
    for (const auto &restaurant : restaurants_) {
      if (restaurant->getDistrict()->getName() == distName) {
        if (foodName.empty() || restaurant->hasFood(foodName)) {
          districtRestaurants.push_back(restaurant->getName());
        }
      }
    }
    sort(districtRestaurants.begin(), districtRestaurants.end());
    for (const auto &restName : districtRestaurants) {
      filteredRestaurants.emplace_back(restName, distName);
    }
  }

  return filteredRestaurants;
}

Response ReservationSystem::getRestaurantDetail(const string &restaurantName) {
  if (!isLoggedIn()) {
    return PermissionDeniedResponse();
  }

  shared_ptr<Restaurant> restaurant = findRest(restaurantName);
  if (!restaurant) {
    return NotFoundResponse();
  }

  return Response(formatRestDetails(restaurant));
}

shared_ptr<Restaurant>
ReservationSystem::findRest(const string &restaurantName) {
  auto restaurantIt =
      std::find_if(restaurants_.begin(), restaurants_.end(),
                   [&restaurantName](const shared_ptr<Restaurant> &rest) {
                     return rest->getName() == restaurantName;
                   });
  return restaurantIt != restaurants_.end() ? *restaurantIt : nullptr;
}

bool ReservationSystem::hasTableConflict(const Table &table, int startTime,
                                         int endTime) const {
  for (const auto &reservation : table.getReservations()) {
    if (!(endTime <= reservation.getStartTime() ||
          startTime >= reservation.getEndTime())) {
      return true;
    }
  }
  return false;
}

bool ReservationSystem::hasUserConflict(const User *user, int startTime,
                                        int endTime,
                                        const string &excludeRestaurant) const {
  for (const auto &reservation : user->getReservations()) {
    if (reservation.getRestaurantName() != excludeRestaurant &&
        !(endTime <= reservation.getStartTime() ||
          startTime >= reservation.getEndTime())) {
      return true;
    }
  }
  return false;
}

Response ReservationSystem::reserve(const string &restaurantName, int tableId,
                                    int startTime, int endTime,
                                    const vector<string> &foods) {

  if (!isValidTimeRange(startTime, endTime)) {
    return PermissionDeniedResponse();
  }

  shared_ptr<Restaurant> restaurant = findRest(restaurantName);
  if (!restaurant) {
    return NotFoundResponse();
  }

  if (!isWithinRestaurantHours(restaurant, startTime, endTime)) {
    return PermissionDeniedResponse();
  }

  shared_ptr<Table> table = findTable(restaurant, tableId);
  if (!table) {
    return NotFoundResponse();
  }

  if (hasTableConflict(*table, startTime, endTime)) {
    return PermissionDeniedResponse();
  }

  if (hasUserConflict(currentUser_, startTime, endTime)) {
    return PermissionDeniedResponse();
  }
  PriceInfo prices{0, 0, 0, 0, 0, 0};
  if (!foods.empty()) {
    auto [isValid, tempPrices] = validateAndCalculateOrder(
        restaurant, foods, currentUser_->isFirstOrder(restaurantName));
    if (!isValid) {
      return NotFoundResponse();
    }
    prices = tempPrices;
  }

  if (prices.finalPrice > currentUser_->getBudget()) {
    return BadRequestResponse();
  } else {
    currentUser_->discharge(prices.finalPrice);
  }

  int reservationId = getNextReservationId(restaurant);
  Reservation newReservation(reservationId, restaurantName, tableId, startTime,
                             endTime, foods);
  newReservation.setPrice(prices);

  currentUser_->addReservation(newReservation);
  restaurant->addReservation(tableId, newReservation);
  string responseOut = formatReservationResponse(newReservation);

  return Response(responseOut);
}

Response ReservationSystem::getReserves(const string &restaurantName,
                                        int reserveId) {
  if (restaurantName.empty()) {
    return formatAllReservations();
  }

  auto restaurant = findRest(restaurantName);
  if (!restaurant) {
    return NotFoundResponse();
  }

  if (reserveId == -1) {
    return formatRestaurantReservations(restaurant);
  }

  return formatSingleReservation(restaurant, reserveId);
}

Response ReservationSystem::formatAllReservations() {
  vector<Reservation> allReservations;

  for (const auto &reservation : currentUser_->getReservations()) {
    allReservations.push_back(reservation);
  }

  if (allReservations.empty()) {
    return EmptyResponse();
  }

  sort(allReservations.begin(), allReservations.end(),
       [](const Reservation &a, const Reservation &b) {
         return a.getStartTime() < b.getStartTime();
       });

  string response;
  for (size_t i = 0; i < allReservations.size(); ++i) {
    response += formatReservationDetails(allReservations[i]);
    if (i < allReservations.size() - 1) {
      response += "\n";
    }
  }

  return response.empty() ? NotFoundResponse() : response;
}

Response ReservationSystem::formatRestaurantReservations(
    const shared_ptr<Restaurant> &restaurant) {
  string response;
  vector<Reservation> restaurantReservations;

  for (const auto &reservation : currentUser_->getReservations()) {
    if (reservation.getRestaurantName() == restaurant->getName()) {
      restaurantReservations.push_back(reservation);
    }
  }

  if (restaurantReservations.empty()) {
    return EmptyResponse();
  }

  sort(restaurantReservations.begin(), restaurantReservations.end(),
       [](const Reservation &a, const Reservation &b) {
         return a.getStartTime() < b.getStartTime();
       });

  for (size_t i = 0; i < restaurantReservations.size(); ++i) {
    response += formatReservationDetails(restaurantReservations[i]);
    if (i < restaurantReservations.size() - 1) {
      response += "\n";
    }
  }

  return response;
}

Response ReservationSystem::formatSingleReservation(
    const shared_ptr<Restaurant> &restaurant, int reserveId) {

  for (const auto &table : restaurant->getTables()) {
    for (const auto &reservation : table.getReservations()) {
      if (reservation.getId() == reserveId) {
        for (const auto &userReservation : currentUser_->getReservations()) {
          if (userReservation.getId() == reserveId &&
              userReservation.getRestaurantName() == restaurant->getName()) {
            return formatReservationDetails(userReservation);
          }
        }
        return PermissionDeniedResponse();
      }
    }
  }
  return NotFoundResponse();
}

Response ReservationSystem::showBudget() {
  return Response(to_string(currentUser_->getBudget()));
}
Response ReservationSystem::increaseBudget(int amount) {
  currentUser_->increaseBudget(amount);
  return OkResponse();
}

Response ReservationSystem::deleteReserve(const string &restaurantName,
                                          int reserveId) {
  auto restaurant = findRest(restaurantName);
  if (!restaurant) {
    return NotFoundResponse();
  }

  bool reservationExists = false;
  for (const auto &table : restaurant->getTables()) {
    for (const auto &reservation : table.getReservations()) {
      if (reservation.getId() == reserveId) {
        reservationExists = true;
      }
    }
  }

  if (!reservationExists) {
    return NotFoundResponse();
  }

  auto &userReservations = currentUser_->getReservations();
  auto resIt = find_if(userReservations.begin(), userReservations.end(),
                       [&](const Reservation &res) {
                         return res.getRestaurantName() == restaurantName &&
                                res.getId() == reserveId;
                       });

  if (resIt == userReservations.end()) {
    return PermissionDeniedResponse();
  }

  Table *table = restaurant->findTable(resIt->getTableId());
  if (!table) {
    return NotFoundResponse();
  }

  table->removeReservation(reserveId);
  currentUser_->removeReservation(restaurantName, reserveId);
  currentUser_->increaseBudget(resIt->getPrice().finalPrice * 60 / 100);
  return OkResponse();
}
