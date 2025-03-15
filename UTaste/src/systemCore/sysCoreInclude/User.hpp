#ifndef User_HPP
#define User_HPP

#include "District.hpp"
#include "Reservation.hpp"
#include <algorithm>
#include <memory>
#include <unordered_set>

class User {
private:
  string uname_;
  string pw_;
  shared_ptr<District> currentDist_;
  vector<Reservation> resis_;
  unordered_set<string> restaurantsVisited_;
  int budget_;

public:
  User(const string &username, const string &password)
      : uname_(username), pw_(password), currentDist_(nullptr), budget_(0) {}

  string getName() { return uname_; }
  string getPsw() { return pw_; }
  void setLocation(shared_ptr<District> district) { currentDist_ = district; }
  shared_ptr<District> getCurrentDistrict() const { return currentDist_; }

  void addReservation(const Reservation &res) {
    resis_.push_back(res);
    addRestaurantVisit(res.getRestaurantName());
  }
  const vector<Reservation> &getReservations() const { return resis_; }
  void removeReservation(const string &restaurantName, int reserveId) {
    auto it =
        find_if(resis_.begin(), resis_.end(), [&](const Reservation &res) {
          return res.getRestaurantName() == restaurantName &&
                 res.getId() == reserveId;
        });
    if (it != resis_.end()) {
      resis_.erase(it);
    }
  }
  bool isFirstOrder(const string &restaurantName) const {
    return restaurantsVisited_.find(restaurantName) ==
           restaurantsVisited_.end();
  }

  void addRestaurantVisit(const string &restaurantName) {
    restaurantsVisited_.insert(restaurantName);
  }
  int getBudget() const { return budget_; }
  void increaseBudget(int amount) { budget_ += amount; }
  void discharge(int amount) { budget_ -= amount; }
};
#endif