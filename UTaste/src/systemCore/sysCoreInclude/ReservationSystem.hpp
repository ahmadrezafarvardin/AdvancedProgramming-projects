#ifndef RESERVATIONSYSTEM_HPP
#define RESERVATIONSYSTEM_HPP

#include "../../communication/communicationInclude/Response.hpp"
#include "../../dataLoader/DataLoader.hpp"
#include "../../general/utilities.hpp"
#include "Restaurant.hpp"
#include "User.hpp"

#include <algorithm>
#include <iostream>
#include <memory>
#include <queue>
#include <unordered_map>
#include <unordered_set>

class ReservationSystem {
private:
  vector<shared_ptr<Restaurant>> restaurants_;
  vector<User> users_;
  User *currentUser_;
  vector<District> districts_;
  unique_ptr<DataLoader> dataLoader_;
  unordered_map<string, shared_ptr<District>> districtMap_;
  unordered_set<string> usernames_;
  vector<string> getBFSDists(const string &startDistrict) const;
  vector<pair<string, string>>
  filterSortRests(const vector<string> &orderedDistricts,
                  const string &foodName) const;
  shared_ptr<Restaurant> findRest(const string &restaurantName);
  bool hasTableConflict(const Table &table, int startTime, int endTime) const;

  bool hasUserConflict(const User *user, int startTime, int endTime,
                       const string &excludeRestaurant = "") const;
  int getNextReservationId(const shared_ptr<Restaurant> &restaurant) const {
    return restaurant->getNextReservationId();
  }
  Response formatAllReservations();
  Response
  formatRestaurantReservations(const shared_ptr<Restaurant> &restaurant);
  Response formatSingleReservation(const shared_ptr<Restaurant> &restaurant,
                                   int reserveId);

public:
  ReservationSystem(unique_ptr<DataLoader> dataLoader)
      : currentUser_(nullptr), dataLoader_(std::move(dataLoader)) {}

  Response getDistricts(const string &districtName = "");
  Response signup(const string &username, const string &password);
  Response login(const string &username, const string &password);
  Response logout();
  Response setUserDistrict(const string &districtName);
  Response getRestaurants(const string &foodName = "");
  Response getRestaurantDetail(const string &restaurantName);
  Response reserve(const string &restaurantName, int tableId, int startTime,
                   int endTime, const vector<string> &foods);
  Response getReserves(const string &restaurantName = "", int reserveId = -1);
  Response deleteReserve(const string &restaurantName, int reserveId);
  Response showBudget();
  Response increaseBudget(int amount);

  void initialize(const string &restaurantsFile, const string &districtsFile,
                  const string &discountFile);
  bool isLoggedIn() { return currentUser_ != nullptr; }
};
#endif